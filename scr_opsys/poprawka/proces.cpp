#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <deque>

using namespace std;

// struktura procesu
struct Process {
    int id = -1;
    int priority = -1;
    int executionTime = -1;
    int termin =-1;
};


void RR(vector<deque<Process>>& processors, int quantum, queue<Process>& queue_to_do, int krok) {
    // zmienna statyczna aby zachować stan między kolejnymi wywołaniami
    static int current_time = 0; 
    int num_processors = processors.size();
    // zmienna pomocnicza all_empty, która określa, czy wszystkie procesory są puste.
    bool all_empty = false;
    // czas następnego zadania (zostawione na wypadek, gdyby kolejka procesów była pusta). 
    int next_task_time = 0;
    
    // dopóki nie skończyły się procesy do wykonania lub procesory nie są puste
    all_empty = true;
    for (int i = 0; i < num_processors; i++) {
        // przypisanie kolejki procesów dla procesora i.
        deque<Process>& q = processors[i];
        if (!q.empty()) {
            // przypisanie wartości false do all_empty, jeśli kolejka nie jest pusta.
            all_empty = false;
            // pobranie pierwszego procesu z kolejki.
            Process current_process = q.front();
            // usunięcie pierwszego procesu z kolejki.
            q.pop_front();
            // aktualizacja pozostałego czasu wykonywania procesu.
            current_process.executionTime = current_process.executionTime -1;
            
            // jeśli pozostało jeszcze czasu, dodaj proces z powrotem do kolejki
            if (current_process.executionTime > 0) {
                if(quantum != 0 && krok% quantum == 0){
                    // dodanie procesu z powrotem do kolejki procesów.
                    q.push_back(current_process);
                }
                else{
                    // dodanie procesu z powrotem do kolejki procesów.
                    q.push_front(current_process);
                }
            }

        }
    }

    if(quantum != 0 && krok% quantum == 0){
        // dodaj nowe procesy do procesorów, jeśli jest ich wystarczająco wiele
        while (!queue_to_do.empty()) {
            // Zmienna min_load przechowuje minimalną liczbę procesów, 
            // a zmienna min_index przechowuje indeks procesora o minimalnym obciążeniu.
            int min_load = num_processors, min_index = -1;
            for (int i = 0; i < num_processors; i++) {
                if (processors[i].size() < min_load) {
                    //Zmienna min_load przechowuje minimalną liczbę procesów,
                    min_load = processors[i].size();
                    // zmienna min_index przechowuje indeks procesora o minimalnym obciążeniu.
                    min_index = i;
                }
            }
            if (min_load < num_processors) {
                // nowy proces do dodania. 1 element z kolejki
                Process new_process = queue_to_do.front();
                // zdjęcie 1 elementu z kolejki
                queue_to_do.pop();
                // Dodanie zadania do procesora z najmniejszą liczbą procesow
                processors[min_index].push_back(new_process);
            } else {
                break;
            }
        }

        // równoważenie obciążenia procesorów przez migrację procesów
        for (int i = 0; i < num_processors; i++) {
            deque<Process>& q = processors[i];
            // Jeśli liczba procesów w kolejce dla danego procesora jest większa niż 2,
            //to procesy są pobierane z kolejki i dodawane do tymczaseowego vectora processes
            if (q.size() >= 2) {
                // rozmiar kolejki 
                int size = q.size();
                // tymczasowy vector processes na którym zostaną umieszczone zadania do przeniesienia
                vector<Process> processes;
                for (int j = 0; j < size; j++) {
                    // dodanie na koniec vectora zadania z początku kolejki
                    processes.push_back(q.front());
                    // usunięcie 1 zadania z kolejki
                    q.pop_front();
                }
                // określenie procesora o mniejszym obciążeniu
                int min_load = num_processors, min_index = -1;
                for (int j = 0; j < num_processors; j++) {
                    if (processors[j].size() < min_load) {
                        //Zmienna min_load przechowuje minimalną liczbę procesów,
                        min_load = processors[j].size();
                        // zmienna min_index przechowuje indeks procesora o minimalnym obciążeniu.
                        min_index = j;
                    }
                }
                // dodanie procesów do procesora docelowego
                for (int j = 0; j < size; j++) {
                    if (i != min_index || j != size - 1) {
                        processors[min_index].push_back(processes[j]);
                    } else {
                        // ostatni proces zostaje na swoim procesorze
                        q.push_back(processes[j]);
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
    cout << "Usage: ./proces <numProcessors> <timeQuantum>" << endl;
    return 1;
    }
    int numProcessors = atoi(argv[1]);
    int timeQuantum = atoi(argv[2]);
    queue<Process> process_to_do;
    vector<deque<Process>> processors_global(numProcessors);

    int i=0;
    std::string line;
    std::stringstream linestream;
    bool getLine = true;


    int t, count_of_process, id, priority, executionTime, deadline;
    int start_time =0;
    while(!(cin.eof())){
        if(getLine){
            std::getline(cin, line);
        }
        if(line!="")
        {
            getLine = false;
            linestream.clear();
            linestream.str(line);
            linestream >> t >> count_of_process;
            if(start_time == t){
                while(!linestream.eof())
                {
                    linestream >> id >> priority >> executionTime >> deadline;
                    process_to_do.push({id, priority, executionTime});
                }
                getLine = true;
            }
            RR(processors_global, timeQuantum, process_to_do, start_time );
            cout << start_time << " ";
            for (int i = 0; i < processors_global.size(); i++) {
                deque<Process> q = processors_global[i];
                if(q.empty()){
                        cout << "-1" << " ";
                    }      
                    else{
                        Process p = q.front();
                        cout << p.id << " ";
                    }
            }
            cout << endl;
            start_time++;
        }
    }
    while(true){
        int vector_empty = true;
        for(int i = 0; i < processors_global.size(); i++){
            if(processors_global[i].empty() == false){
                vector_empty = false;
                break;
            }
        }
        if(process_to_do.empty() && vector_empty){
            break;
        }
        else{
            RR(processors_global, timeQuantum, process_to_do,start_time);
            cout << start_time << " ";
            for (int i = 0; i < processors_global.size(); i++) {
                deque<Process> q = processors_global[i];
                if(q.empty()){
                        cout << "-1" << " ";
                }      
                else{
                    Process p = q.front();
                    cout << p.id << " ";
                }
            }
            cout << endl;
            start_time++;
        }
    }

    return 0;
}

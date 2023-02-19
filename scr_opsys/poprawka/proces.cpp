#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// struktura procesu
struct Process {
    int id = -1;
    int priority = -1;
    int executionTime = -1;
};


void RR(vector<queue<Process>>& processors, int quantum, queue<Process>& queue_to_do) {
    static int current_time = 0; // zmienna statyczna aby zachować stan między kolejnymi wywołaniami
    int num_processors = processors.size();
    bool all_empty = false;
    int next_task_time = 0;

    // dopóki nie skończyły się procesy do wykonania lub procesory nie są puste
    all_empty = true;
    for (int i = 0; i < num_processors; i++) {
        queue<Process>& q = processors[i];
        if (!q.empty()) {
            all_empty = false;

            Process current_process = q.front();
            q.pop();

            int remaining_time = current_process.executionTime - quantum;
            current_time += quantum;

            // jeśli pozostało jeszcze czasu, dodaj proces z powrotem do kolejki
            if (remaining_time > 0) {
                current_process.executionTime = remaining_time;
                q.push(current_process);
            } else {
                // proces zakończony
                cout << "Procesor " << i << ": proces " << current_process.id 
                    << " zakończony w czasie " << current_time << endl;
            }
        }
    }

    // dodaj nowe procesy do procesorów, jeśli jest ich wystarczająco wiele
    while (!queue_to_do.empty()) {
        int min_load = num_processors, min_index = -1;
        for (int i = 0; i < num_processors; i++) {
            if (processors[i].size() < min_load) {
                min_load = processors[i].size();
                min_index = i;
            }
        }
        if (min_load < num_processors) {
            Process new_process = queue_to_do.front();
            queue_to_do.pop();
            processors[min_index].push(new_process);
            cout << "Proces " << new_process.id << " dodany do procesora " << min_index 
                << " w czasie " << current_time << endl;
        } else {
            break;
        }
    }

    // równoważenie obciążenia procesorów przez migrację procesów
    for (int i = 0; i < num_processors; i++) {
        queue<Process>& q = processors[i];
        if (q.size() >= 2) {
            int size = q.size();
            vector<Process> processes;
            for (int j = 0; j < size; j++) {
                processes.push_back(q.front());
                q.pop();
            }
            // określenie procesora o mniejszym obciążeniu
            int min_load = num_processors, min_index = -1;
            for (int j = 0; j < num_processors; j++) {
                if (processors[j].size() < min_load) {
                    min_load = processors[j].size();
                    min_index = j;
                }
            }
            // dodanie procesów do procesora docelowego
            for (int j = 0; j < size; j++) {
                if (i != min_index || j != size - 1) {
                    processors[min_index].push(processes[j]);
                    cout << "Proces " << processes[j].id << " przeniesiony z procesora " << i 
                        << " do procesora" << min_index << " w czasie " << current_time << endl;
                } else {
                // ostatni proces zostaje na swoim procesorze
                q.push(processes[j]);
                }
            }
        }
    }
    // // jeśli w kolejce do zrobienia są jeszcze jakieś procesy, ustaw czas na czas następnego procesu
    // if (!queue_to_do.empty()) {
    //     next_task_time = queue_to_do.front().arrivalTime;
    // }

    // wyświetlanie informacji o aktualnym czasie i stanie procesorów
    cout << "Czas: " << current_time << endl;
    for (int i = 0; i < num_processors; i++) {
        queue<Process>& q = processors[i];
        if (q.empty()) {
            cout << "Procesor " << i << ": pusty" << endl;
        } else {
            cout << "Procesor " << i << ": proces " << q.front().id 
                << " czas wykonania: " << q.front().executionTime << endl;
        }
    }

    // wyświetlanie informacji o czasie do następnego procesu
    if (next_task_time > 0) {
        cout << "Czas do następnego procesu: " << next_task_time - current_time << endl;
    } else {
        cout << "Nie ma więcej procesów do wykonania." << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
    cout << "Usage: ./scheduler <numProcessors> <timeQuantum> <inputFile> <outputFile>" << endl;
    return 1;
    }
    int numProcessors = atoi(argv[1]);
    int timeQuantum = atoi(argv[2]);
    queue<Process> process_to_do;
    vector<queue<Process>> processors_global(numProcessors);

    ifstream inputFile(argv[3]);
    ofstream outputFile(argv[4]);
    int i=0;
    std::string line;
    std::stringstream linestream;
    bool getLine = true;

    if (inputFile.is_open()) {
        int t, count_of_process, id, priority, executionTime;
        int start_time =0;
        while(!(inputFile.eof())){
            if(getLine){
                std::getline(inputFile, line);
            }
            if(line!="")
            {
                getLine = false;
                linestream.clear();
                linestream.str(line);
                // cout<< line<<endl;
                linestream >> t >> count_of_process;
                if(start_time == t){
                    while(!linestream.eof())
                    {
                        linestream >> id >> priority >> executionTime;
                        process_to_do.push({id, priority, executionTime});
                    }
                    getLine = true;
                }
                RR(processors_global, timeQuantum, process_to_do );
                // cout<< processors_global.size()<<endl;
                if (outputFile.is_open()) {
                    outputFile << start_time << " ";
                    for (int i = 0; i < processors_global.size(); i++) {
                        queue<Process> q = processors_global[i];
                        if(q.empty()){
                            outputFile << "-1" << " ";
                        }      
                        else{
                            Process p = q.front();
                            outputFile << p.id << " ";
                        }
                    }
                    outputFile << endl;
                }
                start_time++;
            }
        }
        int i = 0;
        while(true){
            if(i == 30){
                break;
            }
            else{
                RR(processors_global, timeQuantum, process_to_do );
                // cout<< processors_global.size()<<endl;
                if (outputFile.is_open()) {
                    outputFile << start_time << " ";
                    for (int i = 0; i < processors_global.size(); i++) {
                        queue<Process> q = processors_global[i];
                        if(q.empty()){
                            outputFile << "-1" << " ";
                        }      
                        else{
                            Process p = q.front();
                            outputFile << p.id << " ";
                        }
                    }
                    outputFile << endl;
                }
                start_time++;
            }
            i++;
        }
    }

    inputFile.close();
    outputFile.close();
    return 0;
}

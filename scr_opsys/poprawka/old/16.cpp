#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// struktura procesu
struct Process {
    int id;
    int priority;
    int executionTime;
};


// funkcja do wczytywania danych wejściowych z pliku
vector<Process> readInput(string filename) {
    vector<Process> processes;
    ifstream inputFile(filename);
    int i=0;
    std::string line;
    std::stringstream linestream;
    
    if (inputFile.is_open()) {
        int t, count_of_process ,id, priority ,executionTime;
        while(!(inputFile.eof())){
            std::getline(inputFile, line);
            if(line!="")
            {
                linestream.clear();
                linestream.str(line);
                while(!linestream.eof())
                {
                    linestream >> t >> count_of_process >> id >> priority >> executionTime;
            // while (inputFile >> t >> id >> priority >> needed_time >> executionTime) {
                    processes.push_back({id, priority, executionTime});
                }
            }
        }
    }
    inputFile.close();
    return processes;
}


// funkcja do zapisywania wyników do pliku
void writeOutput(string filename, vector<vector<Process>> scheduling) {
    ofstream outputFile(filename);

    if (outputFile.is_open()) {
        for (int i = 0; i < scheduling.size(); i++) {
            outputFile << "Processor " << i << ": ";
            outputFile << endl;
            for (int j = 0; j < scheduling[i].size(); j++) {
                outputFile << "    " << "Task_id:" <<scheduling[i][j].id << " " << scheduling[i][j].priority << " " <<scheduling[i][j].executionTime << " ";
                outputFile << endl;
            }
            outputFile << endl;
        }
    }

    outputFile.close();
}

// funkcja porównująca procesy względem priorytetów
struct ComparePriority {
    bool operator()(const Process& p1, const Process& p2) {
        return p1.priority < p2.priority;
    }
};


// funkcja szeregująca procesy metodą Round Robin
vector<vector<Process>> roundRobinScheduling(vector<Process> processes, int numProcessors, int timeQuantum) {
    vector<vector<Process>> scheduling(numProcessors);
    vector<int> processPosition(processes.size(), 0); // pozycja każdego procesu na swojej kolejce
    int currentTime = 0;

    while (true) {
        // sprawdzenie czy wszystkie procesy zostały wykonane
        bool allProcessesFinished = true;

        // przejście po wszystkich procesach
        for (int i = 0; i < processes.size(); i++) {
            // sprawdzenie czy proces już został wykonany
            if (processPosition[i] >= processes[i].executionTime) {
                continue;
            }

            // wybór kolejnego procesora do wykonania
            int processorIndex = i % numProcessors;

            // wykonanie procesu przez czas kwantu czasu
            for (int j = 0; j < timeQuantum; j++) {
                // sprawdzenie czy proces już został wykonany
                if (processPosition[i] >= processes[i].executionTime) {
                    break;
                }

                // migracja procesu, jeśli potrzebna
                if (scheduling[processorIndex].empty()) {
                    // jeśli kolejka procesora jest pusta, przepisz proces do niej
                    scheduling[processorIndex].push_back(processes[i]);
                } else {
                    // w przeciwnym razie sprawdź, czy można przemieścić proces do innego procesora
                    int targetProcessorIndex = (processorIndex + 1) % numProcessors;
                    bool processMigrated = false;

                    for (int k = 0; k < numProcessors; k++) {
                        if (k == processorIndex || k == targetProcessorIndex) {
                            continue;
                        }

                        if (scheduling[k].size() < scheduling[processorIndex].size()) {
                            // przemieść proces do procesora o mniejszej liczbie procesów w kolejce
                            scheduling[k].push_back(scheduling[processorIndex].back());
                            scheduling[processorIndex].pop_back();
                            scheduling[processorIndex].push_back(processes[i]);
                            processMigrated = true;
                            break;
                        }
                    }

                    if (!processMigrated) {
                        // nie udało się przemieścić procesu do innego procesora, przepisz go do kolejki aktualnego procesora
                        scheduling[processorIndex].push_back(processes[i]);
                    }
                }

                // zaktualizowanie pozycji procesu na swojej kolejce
                processPosition[i]++;

                // zaktualizowanie bieżącego czasu
                currentTime++;
            }
        }

        // sprawdzenie czy wszystkie procesy zostały wykonane
        allProcessesFinished = true;
        for (int i = 0; i < processPosition.size(); i++) {
            if (processPosition[i] < processes[i].executionTime) {
                allProcessesFinished = false;
                break;
            }
        }

        // jeśli wszystkie procesy zostały wykonane, zakończ szeregowanie
        if (allProcessesFinished) {
            break;
        }
    }

    return scheduling;
}

int main(int argc, char* argv[]) {
if (argc != 5) {
cout << "Usage: ./scheduler <numProcessors> <timeQuantum> <inputFile> <outputFile>" << endl;
return 1;
}
int numProcessors = atoi(argv[1]);
int timeQuantum = atoi(argv[2]);
vector<Process> processes = readInput(argv[3]);

while()

// szeregowanie procesów metodą Round Robin
vector<vector<Process>> scheduling = roundRobinScheduling(processes, numProcessors, timeQuantum);

// zapisywanie wyników do pliku
writeOutput(argv[4], scheduling);

return 0;
}

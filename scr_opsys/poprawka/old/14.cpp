#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>

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

    if (inputFile.is_open()) {
        int id, priority, executionTime;
        while (inputFile >> id >> priority >> executionTime) {
            processes.push_back({id, priority, executionTime});
        }
    }

    inputFile.close();
    return processes;
}

// funkcja do zapisywania wyników do pliku
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

// funkcja szeregująca procesy na podstawie priorytetów
vector<vector<Process>> priorityScheduling(vector<Process> processes, int numProcessors) {
    vector<vector<Process>> scheduling(numProcessors);
    priority_queue<Process, vector<Process>, ComparePriority> processQueue;

    // dodanie wszystkich procesów do kolejki
    for (int i = 0; i < processes.size(); i++) {
        processQueue.push(processes[i]);
    }

    // szeregowanie procesów na procesory
    for (int i = 0; i < processes.size(); i++) {
        Process p = processQueue.top();
        processQueue.pop();

        // wybór procesora z najmniejszym obciążeniem
        int minLoadProcessor = 0;
        for (int j = 1; j < numProcessors; j++) {
            if (scheduling[j].size() < scheduling[minLoadProcessor].size()) {
                minLoadProcessor = j;
            }
        }

        // dodanie procesu na wybrany procesor
        scheduling[minLoadProcessor].push_back(p);
    }

    return scheduling;
}

// funkcja szeregująca procesy metodą Round Robin
vector<vector<Process>> roundRobinScheduling(vector<Process> processes, int numProcessors, int timeQuantum) {
    vector<vector<Process>> scheduling(numProcessors);
    vector<int> processPosition(processes.size(), 0); // pozycja każdego procesu na swojej kolejce
    int currentTime = 0;


while (true) {
    // sprawdzenie czy wszystkie procesy zostały wykonane
    bool allProcessesFinished = true;

    // obliczenie bieżącego obciążenia każdego procesora
    vector<int> processorLoad(numProcessors, 0);
    for (int i = 0; i < numProcessors; i++) {
        for (int j = 0; j < scheduling[i].size(); j++) {
            processorLoad[i] += scheduling[i][j].executionTime - processPosition[scheduling[i][j].id];
        }
    }

    for (int i = 0; i < processes.size(); i++) {
        // sprawdzenie czy proces już został wykonany
        if (processPosition[i] >= processes[i].executionTime) {
            continue;
        }

        // wybór kolejnego procesora do wykonania
        int processorIndex = i % numProcessors;

        // sprawdzenie czy bieżące obciążenie procesora nie przekracza maksymalnego progu
        if (processorLoad[processorIndex] > 2 * timeQuantum) {
            // poszukaj procesów w kolejce innego procesora, których wykonanie jeszcze się nie zakończyło
            int shortestRemainingTime = INT_MAX;
            int shortestRemainingTimeIndex = -1;
            for (int j = 0; j < numProcessors; j++) {
                if (j == processorIndex) {
                    continue;
                }
                for (int k = 0; k < scheduling[j].size(); k++) {
                    int remainingTime = scheduling[j][k].executionTime - processPosition[scheduling[j][k].id];
                    if (remainingTime > 0 && remainingTime < shortestRemainingTime) {
                        shortestRemainingTime = remainingTime;
                       
                                            shortestRemainingTimeIndex = j;
                    }
                }
            }

            // jeśli znaleziono proces do przeniesienia, przenieś go do kolejki procesora o najniższym obciążeniu
            if (shortestRemainingTimeIndex != -1) {
                Process processToMove = scheduling[shortestRemainingTimeIndex].back();
                scheduling[shortestRemainingTimeIndex].pop_back();
                scheduling[processorIndex].push_back(processToMove);
                processorLoad[processorIndex] += processToMove.executionTime - processPosition[processToMove.id];
                processorLoad[shortestRemainingTimeIndex] -= processToMove.executionTime - processPosition[processToMove.id];
            }
        }

        // wykonanie procesu przez czas kwantu czasu
        for (int j = 0; j < timeQuantum; j++) {
            // sprawdzenie czy proces już został wykonany
            if (processPosition[i] >= processes[i].executionTime) {
                break;
            }

            // dodanie procesu do planisty procesora
            scheduling[processorIndex].push_back(processes[i]);

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

// szeregowanie procesów metodą Round Robin
vector<vector<Process>> scheduling = roundRobinScheduling(processes, numProcessors, timeQuantum);

// zapisywanie wyników do pliku
writeOutput(argv[4], scheduling);

return 0;
}

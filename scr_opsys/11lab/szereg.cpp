
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;
#define DATAFILE "./sched1.in.txt"
#define OUTPUT_FCFS "./output1_fcfs"
#define OUTPUT_SJF "./output1_sjf"
#define OUTPUT_PRIO "./output1_prio"

int process_count = 0;
int current_time = 0;
bool isBusy = 0;
int current_index;

struct process
{
    int id, at, bt, prio, start_time = -1;
    bool isDone = false;
} ps[100];

int scheduleOneLine(char choice)
{

    switch (choice)
    {
    case '0': // fcfs
        for (int i = 0; i < process_count; i++)
        {
            if (!isBusy && ps[i].start_time == -1)
            {
                ps[i].start_time = current_time;
                isBusy = true;
                return ps[i].id;
            }
            if (ps[i].start_time != -1)
            {
                if (current_time - ps[i].start_time != ps[i].bt && !ps[i].isDone)
                {
                    isBusy = true;
                    return ps[i].id;
                }
                if (current_time - ps[i].start_time == ps[i].bt)
                {
                    ps[i].isDone = true;
                    isBusy = false;
                }
            }
        }
        return -1;
        break;
    case '1': // sjf
    {
        if (current_time - ps[current_index].start_time == ps[current_index].bt)
        {
            ps[current_index].isDone = true;
            isBusy = false;
        }
        if (!isBusy)
        {
            int indexOfMin = -1;
            int burstOfMin = 0;
            for (int i = 0; i < process_count; i++)
            {
                if ((indexOfMin == -1 || burstOfMin > ps[i].bt) && !ps[i].isDone)
                {
                    indexOfMin = i;
                    burstOfMin = ps[i].bt;
                }
            }
            if (indexOfMin == -1)
                return -1;
            ps[indexOfMin].start_time = current_time;
            isBusy = true;
            current_index = indexOfMin;
            return ps[indexOfMin].id;
        }
        if (current_time - ps[current_index].start_time != ps[current_index].bt && !ps[current_index].isDone)
        {
            isBusy = true;
            return ps[current_index].id;
        }
        return -1;
    }
    break;
    case '2': // prio
    {
        if (current_time - ps[current_index].start_time == ps[current_index].bt)
        {
            ps[current_index].isDone = true;
            isBusy = false;
        }
        if (!isBusy)
        {
            int indexOfMin = -1;
            int prioOfMin;
            for (int i = 0; i < process_count; i++)
            {
                if ((indexOfMin == -1 || prioOfMin > ps[i].prio) && !ps[i].isDone)
                {
                    indexOfMin = i;
                    prioOfMin = ps[i].prio;
                }
            }
            if (indexOfMin == -1)
                return -1;
            ps[indexOfMin].start_time = current_time;
            isBusy = true;
            current_index = indexOfMin;
            return ps[indexOfMin].id;
        }
        if (current_time - ps[current_index].start_time != ps[current_index].bt && !ps[current_index].isDone)
        {
            isBusy = true;
            return ps[current_index].id;
        }
        return -1;
    }
    break;
    default:
        cout << "No valid scheduling algorithm\n";
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{

    string line;
    ifstream data;
    data.open(DATAFILE);
    ofstream outputFCFS, outputSJF, outputPRIO;

    switch (argv[1][0])
    {
    case '0': // fcfs
        outputFCFS.open(OUTPUT_FCFS);
        break;
    case '1': // sjf
        outputSJF.open(OUTPUT_SJF);
        break;
    case '2': // prio
        outputPRIO.open(OUTPUT_PRIO);
        break;
    default:
        cout << "No valid scheduling algorithm\n";
        return -2;
    }

    bool marker = 1;
    int result = 0;

    while (marker != 0 && result != -1)
    {
        if (getline(data, line))
        {
            if (!line.empty())
            {
                int size = line.size();
                int index = line.find(" ");
                string a = line.substr(0, index);
                int arrival_time;
                arrival_time = stoi(a);
                line = line.substr(index + 1, line.length());
                for (int i = 0; i < (size) / 6; i++)
                {
                    ps[process_count].at = arrival_time;
                    index = line.find(" ");
                    a = line.substr(0, index);
                    ps[process_count].id = stoi(a);
                    line = line.substr(index + 1, line.length());
                    index = line.find(" ");
                    a = line.substr(0, index);
                    ps[process_count].prio = stoi(a);
                    line = line.substr(index + 1, line.length());
                    index = line.find(" ");
                    a = line.substr(0, index);
                    ps[process_count].bt = stoi(a);
                    line = line.substr(index + 1, line.length());

                    // cout<<"process: "<<process_count<<" id: "
                    // <<ps[process_count].id<<" prio: "
                    // <<ps[process_count].prio<<" bt: "
                    // <<ps[process_count].bt<<"\n";
                    process_count += 1;
                }
            }
            marker = 0;
        }

        result = scheduleOneLine(argv[1][0]);
        switch (argv[1][0])
        {
        case '0': // fcfs
            outputFCFS << current_time << " " << result << "\n";
            break;
        case '1': // sjf
            outputSJF << current_time << " " << result << "\n";
            break;
        case '2': // prio
            outputPRIO << current_time << " " << result << "\n";
            break;
        default:
            cout << "No valid scheduling algorithm\n";
            return -2;
        }
        current_time += 1;
        marker = 1;
    }
    data.close();
    switch (argv[1][0])
    {
    case '0': // fcfs
        outputFCFS.close();
        break;
    case '1': // sjf
        outputSJF.close();
        break;
    case '2': // prio
        outputPRIO.close();
        break;
    default:
        cout << "No valid scheduling algorithm\n";
        return -2;
    }
    return 0;
}
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

            Process current_task = q.front();
            q.pop();

            int remaining_time = current_task.executionTime - quantum;
            current_time += quantum;

            // jeśli pozostało jeszcze czasu, dodaj zadanie z powrotem do kolejki
            if (remaining_time > 0) {
                current_task.executionTime = remaining_time;
                q.push(current_task);
            } else {
                // zadanie zakończone
                cout << "Procesor " << i << ": zadanie " << current_task.id 
                    << " zakończone w czasie " << current_time << endl;
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
            cout << "Zadanie " << new_process.id << " dodane do procesora " << min_index 
                << " w czasie " << current_time << endl;
        } else {
            break;
        }
    }
}

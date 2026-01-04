#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "linked_list.h"

ProcessList* read_input_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }
    
    ProcessList* list = create_list();
    char line[256];
    int process_id = 1;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) continue;
        
        int burst, priority, arrival, queue_id;
        if (sscanf(line, "%d:%d:%d:%d", &burst, &priority, &arrival, &queue_id) == 4) {
            Process* p = create_process(process_id++, burst, priority, arrival, queue_id);
            add_process(list, p);
        } else {
            fprintf(stderr, "Warning: Invalid line format: %s\n", line);
        }
    }
    
    fclose(file);
    return list;
}

ProcessList** separate_by_queue(ProcessList* all_processes, int* queue_count) {
    if (!all_processes || !all_processes->head) {
        *queue_count = 0;
        return NULL;
    }
    
    *queue_count = count_unique_queues(all_processes);
    
    ProcessList** queues = (ProcessList**)malloc(*queue_count * sizeof(ProcessList*));
    if (!queues) {
        perror("Failed to allocate queues array");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < *queue_count; i++) {
        queues[i] = create_list();
    }
    
    Process* current = all_processes->head;
    while (current) {
        if (current->queue_id >= 0 && current->queue_id < *queue_count) {
            Process* copy = create_process(current->id, current->burst_time,
                                          current->priority, current->arrival_time,
                                          current->queue_id);
            add_process(queues[current->queue_id], copy);
        }
        current = current->next;
    }
    
    return queues;
}

int count_unique_queues(ProcessList* list) {
    if (!list || !list->head) return 0;
    
    int max_queue_id = -1;
    Process* current = list->head;
    
    while (current) {
        if (current->queue_id > max_queue_id) {
            max_queue_id = current->queue_id;
        }
        current = current->next;
    }
    
    return max_queue_id + 1;
}

void calculate_metrics(ProcessList* list) {
    if (!list || !list->head) return;
    
    Process* current = list->head;
    int current_time = 0;
    
    while (current) {
        if (current->arrival_time > current_time) {
            current_time = current->arrival_time;
        }
        
        current->waiting_time = current_time - current->arrival_time;
        current->completion_time = current_time + current->burst_time;
        current->turnaround_time = current->completion_time - current->arrival_time;
        
        current_time = current->completion_time;
        current = current->next;
    }
}

float calculate_average_waiting_time(ProcessList* list) {
    if (!list || list->count == 0) return 0.0f;
    
    Process* current = list->head;
    float total_waiting = 0.0f;
    int count = 0;
    
    while (current) {
        total_waiting += current->waiting_time;
        count++;
        current = current->next;
    }
    
    return (count > 0) ? total_waiting / count : 0.0f;
}

void write_output_file(const char* filename, ProcessList** results, int queue_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    
    for (int q = 0; q < queue_count; q++) {
        ProcessList* queue_list = results[q];
        if (!queue_list || queue_list->count == 0) continue;
        
        fprintf(file, "%d:1", q);
        Process* current = queue_list->head;
        while (current) {
            if (current->queue_id == q) {
                fprintf(file, ":%d", current->waiting_time);
            }
            current = current->next;
        }
        float avg_wt = calculate_average_waiting_time(queue_list);
        fprintf(file, ":%.2f\n", avg_wt);
        
    }
    
    fclose(file);
}

void write_to_screen(ProcessList** results, int queue_count) {
    printf("Scheduling Results:\n");
    printf("===================\n");
    
    for (int q = 0; q < queue_count; q++) {
        ProcessList* queue_list = results[q];
        if (!queue_list || queue_list->count == 0) continue;
        
        printf("\nQueue %d:\n", q);
        printf("PID\tBurst\tPriority\tArrival\tWaiting\tTurnaround\n");
        printf("----------------------------------------------------\n");
        
        Process* current = queue_list->head;
        while (current) {
            printf("%d\t%d\t%d\t\t%d\t%d\t%d\n",
                   current->id, current->burst_time, current->priority,
                   current->arrival_time, current->waiting_time,
                   current->turnaround_time);
            current = current->next;
        }
        
        float avg_wt = calculate_average_waiting_time(queue_list);
        printf("Average Waiting Time: %.2f\n", avg_wt);
    }
}

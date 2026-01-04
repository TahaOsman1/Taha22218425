#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "scheduler.h"
#include "utils.h"

void run_simulation(ProcessList* list, const char* output_filename);
void write_results_to_file(const char* filename, ScheduleResult*** results, int queue_count);
void write_results_to_screen(ScheduleResult*** results, int queue_count);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        fprintf(stderr, "Example: ./cpe351 input.txt output.txt\n");
        return EXIT_FAILURE;
    }
    
    const char* input_file = argv[1];
    const char* output_file = argv[2];
    
    printf("CPU Scheduler Simulator\n");
    printf("=======================\n");
    printf("Input file: %s\n", input_file);
    printf("Output file: %s\n", output_file);
    
    ProcessList* all_processes = read_input_file(input_file);
    printf("Read %d processes from input file.\n", all_processes->count);
    
    run_simulation(all_processes, output_file);
    
    free_list(all_processes);
    
    printf("\nSimulation completed successfully.\n");
    return EXIT_SUCCESS;
}

void run_simulation(ProcessList* all_processes, const char* output_filename) {
    int queue_count;
    ProcessList** queues = separate_by_queue(all_processes, &queue_count);
    printf("Found %d queues.\n", queue_count);
    
    ScheduleResult*** all_results = (ScheduleResult***)malloc(queue_count * sizeof(ScheduleResult**));
    if (!all_results) {
        perror("Failed to allocate results array");
        exit(EXIT_FAILURE);
    }
    
    for (int q = 0; q < queue_count; q++) {
        all_results[q] = (ScheduleResult**)malloc(3 * sizeof(ScheduleResult*));
        if (!all_results[q]) {
            perror("Failed to allocate queue results");
            exit(EXIT_FAILURE);
        }
        
        for (int a = 0; a < 3; a++) {
            all_results[q][a] = NULL;
        }
        
        ProcessList* queue_list = queues[q];
        if (!queue_list || queue_list->count == 0) {
            for (int a = 0; a < 3; a++) {
                all_results[q][a] = create_result(q, a + 1, queue_list);
            }
            continue;
        }
        
        printf("\nProcessing Queue %d (%d processes):\n", q, queue_list->count);
        
        printf("  Running FCFS... ");
        ProcessList* fcfs_list = copy_list(queue_list);
        fcfs_schedule(fcfs_list);
        all_results[q][0] = create_result(q, 1, fcfs_list);
        printf("Done. AWT: %.2f\n", all_results[q][0]->average_waiting);
        free_list(fcfs_list);
        
        printf("  Running SJF... ");
        ProcessList* sjf_list = copy_list(queue_list);
        sjf_schedule(sjf_list);
        all_results[q][1] = create_result(q, 2, sjf_list);
        printf("Done. AWT: %.2f\n", all_results[q][1]->average_waiting);
        free_list(sjf_list);
        
        printf("  Running Priority... ");
        ProcessList* priority_list = copy_list(queue_list);
        priority_schedule(priority_list);
        all_results[q][2] = create_result(q, 3, priority_list);
        printf("Done. AWT: %.2f\n", all_results[q][2]->average_waiting);
        free_list(priority_list);
    }
    
    write_results_to_file(output_filename, all_results, queue_count);
    write_results_to_screen(all_results, queue_count);
    
    for (int q = 0; q < queue_count; q++) {
        if (queues[q]) free_list(queues[q]);
        
        for (int a = 0; a < 3; a++) {
            if (all_results[q][a]) free_result(all_results[q][a]);
        }
        free(all_results[q]);
    }
    free(all_results);
    free(queues);
}

void write_results_to_file(const char* filename, ScheduleResult*** results, int queue_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }
    
    for (int q = 0; q < queue_count; q++) {
        for (int a = 0; a < 3; a++) {
            ScheduleResult* result = results[q][a];
            if (!result) continue;
            
            fprintf(file, "%d:%d", result->queue_id, result->algorithm);
            
            for (int i = 0; i < result->process_count; i++) {
                fprintf(file, ":%d", result->waiting_times[i]);
            }
            
            fprintf(file, ":%.2f\n", result->average_waiting);
        }
    }
    
    fclose(file);
    printf("\nResults written to %s\n", filename);
}

void write_results_to_screen(ScheduleResult*** results, int queue_count) {
    printf("\nFinal Results:\n");
    printf("==============\n");
    
    for (int q = 0; q < queue_count; q++) {
        for (int a = 0; a < 3; a++) {
            ScheduleResult* result = results[q][a];
            if (!result) continue;
            
            printf("Queue %d, Algorithm %d: ", result->queue_id, result->algorithm);
            for (int i = 0; i < result->process_count; i++) {
                printf("%d", result->waiting_times[i]);
                if (i < result->process_count - 1) printf(":");
            }
            printf(":%.2f\n", result->average_waiting);
        }
    }
}

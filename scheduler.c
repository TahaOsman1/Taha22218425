#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "linked_list.h"

void fcfs_schedule(ProcessList* list) {
    if (!list || list->count < 1) return;
    
    ProcessList* working_list = copy_list(list);
    
    sort_by_arrival(working_list);
    
    calculate_metrics(working_list);
    
    Process* orig = list->head;
    Process* work = working_list->head;
    
    while (orig && work) {
        if (orig->id == work->id) {
            orig->waiting_time = work->waiting_time;
            orig->turnaround_time = work->turnaround_time;
            orig->completion_time = work->completion_time;
            orig = orig->next;
        }
        work = work->next;
    }
    
    free_list(working_list);
}

void sjf_schedule(ProcessList* list) {
    if (!list || list->count < 1) return;
    
    ProcessList* working_list = copy_list(list);
    ProcessList* scheduled = create_list();
    
    int current_time = 0;
    int total_processes = working_list->count;
    
    while (scheduled->count < total_processes) {
        Process* next = find_min_burst(working_list, current_time);
        
        if (next) {
            remove_process(working_list, next);
            
            if (current_time < next->arrival_time) {
                current_time = next->arrival_time;
            }
            
            next->waiting_time = current_time - next->arrival_time;
            next->completion_time = current_time + next->burst_time;
            next->turnaround_time = next->completion_time - next->arrival_time;
            
            current_time = next->completion_time;
            add_process(scheduled, next);
        } else {
            current_time++;
        }
    }
    
    Process* orig = list->head;
    Process* sched = scheduled->head;
    
    while (orig && sched) {
        if (orig->id == sched->id) {
            orig->waiting_time = sched->waiting_time;
            orig->turnaround_time = sched->turnaround_time;
            orig->completion_time = sched->completion_time;
            orig = orig->next;
        }
        sched = sched->next;
    }
    
    free_list(working_list);
    free_list(scheduled);
}

void priority_schedule(ProcessList* list) {
    if (!list || list->count < 1) return;
    
    ProcessList* working_list = copy_list(list);
    ProcessList* scheduled = create_list();
    
    int current_time = 0;
    int total_processes = working_list->count;
    
    while (scheduled->count < total_processes) {
        Process* next = find_highest_priority(working_list, current_time);
        
        if (next) {
            remove_process(working_list, next);
            
            if (current_time < next->arrival_time) {
                current_time = next->arrival_time;
            }
            
            next->waiting_time = current_time - next->arrival_time;
            next->completion_time = current_time + next->burst_time;
            next->turnaround_time = next->completion_time - next->arrival_time;
            
            current_time = next->completion_time;
            add_process(scheduled, next);
        } else {
            current_time++;
        }
    }
    
    Process* orig = list->head;
    Process* sched = scheduled->head;
    
    while (orig && sched) {
        if (orig->id == sched->id) {
            orig->waiting_time = sched->waiting_time;
            orig->turnaround_time = sched->turnaround_time;
            orig->completion_time = sched->completion_time;
            orig = orig->next;
        }
        sched = sched->next;
    }
    
    free_list(working_list);
    free_list(scheduled);
}

ScheduleResult* create_result(int queue_id, int algorithm, ProcessList* list) {
    if (!list || list->count == 0) return NULL;
    
    ScheduleResult* result = (ScheduleResult*)malloc(sizeof(ScheduleResult));
    if (!result) {
        perror("Failed to create result");
        exit(EXIT_FAILURE);
    }
    
    result->queue_id = queue_id;
    result->algorithm = algorithm;
    result->process_count = list->count;
    
    result->waiting_times = (int*)malloc(list->count * sizeof(int));
    if (!result->waiting_times) {
        perror("Failed to allocate waiting times");
        free(result);
        exit(EXIT_FAILURE);
    }
    
    Process* current = list->head;
    int i = 0;
    float total = 0.0f;
    
    while (current) {
        result->waiting_times[i] = current->waiting_time;
        total += current->waiting_time;
        i++;
        current = current->next;
    }
    
    result->average_waiting = total / list->count;
    return result;
}

void free_result(ScheduleResult* result) {
    if (!result) return;
    if (result->waiting_times) free(result->waiting_times);
    free(result);
}

void print_result(ScheduleResult* result) {
    if (!result) {
        printf("Result is NULL\n");
        return;
    }
    
    printf("Queue %d, Algorithm %d: ", result->queue_id, result->algorithm);
    for (int i = 0; i < result->process_count; i++) {
        printf("%d", result->waiting_times[i]);
        if (i < result->process_count - 1) printf(":");
    }
    printf(":%.2f\n", result->average_waiting);
}

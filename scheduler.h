#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "linked_list.h"

void fcfs_schedule(ProcessList* list);
void sjf_schedule(ProcessList* list);
void priority_schedule(ProcessList* list);

typedef struct {
    int queue_id;
    int algorithm;
    int* waiting_times;
    int process_count;
    float average_waiting;
} ScheduleResult;

ScheduleResult* create_result(int queue_id, int algorithm, ProcessList* list);
void free_result(ScheduleResult* result);
void print_result(ScheduleResult* result);

#endif

#ifndef UTILS_H
#define UTILS_H

#include "linked_list.h"

ProcessList* read_input_file(const char* filename);
void write_output_file(const char* filename, ProcessList** results, int queue_count);
void write_to_screen(ProcessList** results, int queue_count)
void calculate_metrics(ProcessList* list);
float calculate_average_waiting_time(ProcessList* list);

ProcessList** separate_by_queue(ProcessList* all_processes, int* queue_count);
int count_unique_queues(ProcessList* list);

#endif

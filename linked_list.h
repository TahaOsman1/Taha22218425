#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Process {
    int id;                 
    int burst_time;         
    int priority;           
    int arrival_time;       
    int queue_id;           
    int waiting_time;       
    int turnaround_time;    
    int completion_time;    
    struct Process* next;   
} Process;

typedef struct {
    Process* head;
    Process* tail;
    int count;
} ProcessList;

ProcessList* create_list();
void add_process(ProcessList* list, Process* p);
Process* remove_process(ProcessList* list, Process* p);
Process* create_process(int id, int burst, int priority, int arrival, int queue_id);
void free_list(ProcessList* list);
ProcessList* copy_list(ProcessList* original);
void sort_by_arrival(ProcessList* list);
void sort_by_burst(ProcessList* list);
void sort_by_priority(ProcessList* list);
Process* find_min_burst(ProcessList* list, int current_time);
Process* find_highest_priority(ProcessList* list, int current_time);
void print_list(ProcessList* list);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

ProcessList* create_list() {
    ProcessList* list = (ProcessList*)malloc(sizeof(ProcessList));
    if (!list) {
        perror("Failed to create list");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

Process* create_process(int id, int burst, int priority, int arrival, int queue_id) {
    Process* p = (Process*)malloc(sizeof(Process));
    if (!p) {
        perror("Failed to create process");
        exit(EXIT_FAILURE);
    }
    p->id = id;
    p->burst_time = burst;
    p->priority = priority;
    p->arrival_time = arrival;
    p->queue_id = queue_id;
    p->waiting_time = 0;
    p->turnaround_time = 0;
    p->completion_time = 0;
    p->next = NULL;
    return p;
}

void add_process(ProcessList* list, Process* p) {
    if (!list || !p) return;
    
    if (list->head == NULL) {
        list->head = p;
        list->tail = p;
    } else {
        list->tail->next = p;
        list->tail = p;
    }
    list->count++;
}

Process* remove_process(ProcessList* list, Process* p) {
    if (!list || !list->head || !p) return NULL;
    
    Process *current = list->head;
    Process *prev = NULL;
    
    if (current == p) {
        list->head = current->next;
        if (list->tail == p) {
            list->tail = NULL;
        }
        list->count--;
        p->next = NULL;
        return p;
    }
    
    while (current && current != p) {
        prev = current;
        current = current->next;
    }
    
    if (current == p) {
        prev->next = current->next;
        if (list->tail == p) {
            list->tail = prev;
        }
        list->count--;
        p->next = NULL;
        return p;
    }
    
    return NULL;
}

void free_list(ProcessList* list) {
    if (!list) return;
    
    Process* current = list->head;
    while (current) {
        Process* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

ProcessList* copy_list(ProcessList* original) {
    if (!original) return NULL;
    
    ProcessList* copy = create_list();
    Process* current = original->head;
    
    while (current) {
        Process* new_p = create_process(current->id, current->burst_time,
                                        current->priority, current->arrival_time,
                                        current->queue_id);
        add_process(copy, new_p);
        current = current->next;
    }
    
    return copy;
}

void sort_by_arrival(ProcessList* list) {
    if (!list || list->count < 2) return;
    
    int swapped;
    do {
        swapped = 0;
        Process** ptr = &(list->head);
        
        while (*ptr && (*ptr)->next) {
            Process* p1 = *ptr;
            Process* p2 = p1->next;
            
            if (p1->arrival_time > p2->arrival_time ||
                (p1->arrival_time == p2->arrival_time && p1->id > p2->id)) {
                p1->next = p2->next;
                p2->next = p1;
                *ptr = p2;
                swapped = 1;
                
                if (p1->next == NULL) {
                    list->tail = p1;
                }
            }
            ptr = &((*ptr)->next);
        }
    } while (swapped);
}
void sort_by_burst(ProcessList* list) {
    if (!list || list->count < 2) return;
    
    int swapped;
    do {
        swapped = 0;
        Process** ptr = &(list->head);
        
        while (*ptr && (*ptr)->next) {
            Process* p1 = *ptr;
            Process* p2 = p1->next;
            
            if (p1->burst_time > p2->burst_time ||
                (p1->burst_time == p2->burst_time && p1->arrival_time > p2->arrival_time)) {
                p1->next = p2->next;
                p2->next = p1;
                *ptr = p2;
                swapped = 1;
                
                if (p1->next == NULL) {
                    list->tail = p1;
                }
            }
            ptr = &((*ptr)->next);
        }
    } while (swapped);
}

void sort_by_priority(ProcessList* list) {
    if (!list || list->count < 2) return;
    
    int swapped;
    do {
        swapped = 0;
        Process** ptr = &(list->head);
        
        while (*ptr && (*ptr)->next) {
            Process* p1 = *ptr;
            Process* p2 = p1->next;
            
            if (p1->priority > p2->priority ||
                (p1->priority == p2->priority && p1->arrival_time > p2->arrival_time)) {
                p1->next = p2->next;
                p2->next = p1;
                *ptr = p2;
                swapped = 1;
                
                if (p1->next == NULL) {
                    list->tail = p1;
                }
            }
            ptr = &((*ptr)->next);
        }
    } while (swapped);
}

Process* find_min_burst(ProcessList* list, int current_time) {
    if (!list || !list->head) return NULL;
    
    Process* current = list->head;
    Process* min_process = NULL;
    int min_burst = 999999;
    
    while (current) {
        if (current->arrival_time <= current_time && current->burst_time < min_burst) {
            min_burst = current->burst_time;
            min_process = current;
        }
        current = current->next;
    }
    
    return min_process;
}

Process* find_highest_priority(ProcessList* list, int current_time) {
    if (!list || !list->head) return NULL;
    
    Process* current = list->head;
    Process* highest_process = NULL;
    int highest_priority = 999999;
    
    while (current) {
        if (current->arrival_time <= current_time && current->priority < highest_priority) {
            highest_priority = current->priority;
            highest_process = current;
        }
        current = current->next;
    }
    
    return highest_process;
}
void print_list(ProcessList* list) {
    if (!list) {
        printf("List is NULL\n");
        return;
    }
    
    printf("Process List (%d processes):\n", list->count);
    printf("ID\tBurst\tPriority\tArrival\tQueue\tWT\tTAT\tCT\n");
    printf("------------------------------------------------------------\n");
    
    Process* current = list->head;
    while (current) {
        printf("%d\t%d\t%d\t\t%d\t%d\t%d\t%d\t%d\n",
               current->id, current->burst_time, current->priority,
               current->arrival_time, current->queue_id,
               current->waiting_time, current->turnaround_time,
               current->completion_time);
        current = current->next;
    }
    printf("\n");
}

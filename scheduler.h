//
// Created by Ahmed Khairallah
//

#ifndef OS_FINAL_SCHEDULER_H
#define OS_FINAL_SCHEDULER_H
#include "priority_queue.h"
#include "process.h"
#include "node.h"


void initialize_scheduling();
void insertinq(process* p, int next_prim, thread_node * node);
int actualpriority(process* p);
void scheduler(thread_node * node);
void print_finished();

extern pthread_mutex_t print_lock;
extern pthread_mutex_t finished_lock;

#endif //OS_FINAL_SCHEDULER_H

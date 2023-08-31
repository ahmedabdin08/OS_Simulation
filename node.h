//
// Created by Ahmed Khairallah
//

#ifndef OS_FINAL_NODE_H
#define OS_FINAL_NODE_H

#include "priority_queue.h"
#include "pthread.h"

typedef struct thread_node{
    priority_queue_t *blockedq;      //thread blocked queue
    priority_queue_t *readyq;        //thread ready queue
    int clock;               //thread time
    int next_pid;       //next process id
    int id;                 //thread id
} thread_node;
extern unsigned int numberOfNodes, * ids;

//execute thread function to execute the thread and the function will be called by the thread when pthread_create happens
extern void* execute_thread(void* thread_id);


#endif //OS_FINAL_NODE_H

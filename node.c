//
// Created by Ahmed Khairallah
//

#include "node.h"
#include <stdlib.h>
#include "barrier.h"
#include "process.h"
#include "scheduler.h"

//initializing the thread node struct
static thread_node* initialize_node(){
    thread_node * result = (thread_node *)malloc(sizeof(thread_node));
    result->blockedq = initialize_pq();
    result->readyq = initialize_pq();
    result->next_pid = 1;
    result->clock = 0;
    return result;
}
//execute thread function to execute the thread and the function will be called by the thread when pthread_create happens
extern void* execute_thread(void * arg){
    int thread_id = *(int*)arg;
    thread_node * ptr = initialize_node();
    ptr->id = thread_id;
    for(int i = 0; i < numberOfProcesses; i++){
        if(procs[i]->node_id == thread_id){
            procs[i]->pid = ptr->next_pid;
            procs[i]->address = procs[i]->node_id * 100 + procs[i]->pid;
            ptr->next_pid++;
            printProcess(procs[i], ptr);
            insertinq(procs[i], 1, ptr);
        }
    }
    barrier_wait();
    scheduler(ptr);
    return NULL;
}
//
// Created by Ahmed Khairallah
//

#include "communication.h"
#include <assert.h>

pthread_mutex_t * communication_locks = NULL;
//function for finding the processes
static process * find_proc_from_address(unsigned int address){
    unsigned int id = address % 100, node_id = address/100;

    for (int (i) = 0; (i) < numberOfProcesses; i++) {
        if(procs[i]->pid == id && procs[i]->node_id == node_id) {
            return procs[i];
        }
    }
    return NULL;
}

//send and receive functions to send and receive the address and block the process
extern void send(unsigned int receiver_address, thread_node * node, process * sender){
    process * receiver = find_proc_from_address(receiver_address);
    assert(receiver);

    pthread_mutex_lock(&communication_locks[sender->pid]);
    sender->waiting = 1;
    printProcess(sender, node);
    //ticks here are meant to represent the address of the sender
    if(receiver->waiting && receiver->primitiveslist[receiver->index].ticks == sender->address){
        sender->waiting = 0;
        receiver->waiting = 0;
        sender->time_done = node->clock + 1;
        receiver->time_done = node->clock + 1;
    }
    pthread_mutex_unlock(&communication_locks[sender->pid]);
}
extern void receive(unsigned int sender_address, thread_node * node, process * receiver){
    process * sender = find_proc_from_address(sender_address);
    assert(sender);

    pthread_mutex_lock(&communication_locks[receiver->pid]);
    receiver->waiting = 1;
    printProcess(receiver, node);
    //ticks here are meant to represent the address of the receiver
    if(sender->waiting && sender->primitiveslist[sender->index].ticks == receiver->address){
        sender->waiting = 0;
        receiver->waiting = 0;
        sender->time_done = node->clock + 1;
        receiver->time_done = node->clock + 1;
    }
    pthread_mutex_unlock(&communication_locks[receiver->pid]);
}

//done list for the processes that are unblocked
extern priority_queue_t * done_list(thread_node * node){
    priority_queue_t * result = initialize_pq();
    for (int (i) = 0; (i) < numberOfProcesses; i++) {
        if(procs[i]->node_id == node->id && procs[i]->time_done == node->clock){
            enqueue(result, procs[i], procs[i]->pid);
        }
    }
    return result;
}

int is_communicating(thread_node * node){
    for(int i = 0; i < numberOfProcesses; i++){
        if (procs[i]->node_id== node->id && (procs[i]->time_done != 0 || procs[i]->waiting)) {
            return 1;
        }
    }
    return 0;
}
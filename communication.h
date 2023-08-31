//
// Created by Ahmed Khairallah
//

#ifndef OS_FINAL_COMMUNICATION_H
#define OS_FINAL_COMMUNICATION_H
#include "node.h"
#include "priority_queue.h"
#include "process.h"
#include <pthread.h>

extern void send(unsigned int receiver_address, thread_node * node, process * sender);
extern void receive(unsigned int sender_address, thread_node * node, process * receiver);
extern priority_queue_t * done_list(thread_node * node);
int is_communicating(thread_node * node);
#endif //OS_FINAL_COMMUNICATION_H

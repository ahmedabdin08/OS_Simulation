//
// Created by Ahmed Khairallah
//

#ifndef OS_FINAL_PROCESS_H
#define OS_FINAL_PROCESS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

#define MAXSTRLEN 11

enum {READFAIL, READSUCESS};

enum { NEW, READY, RUNNING, BLOCKED, FINISHED };

enum primitivetype { DOOP, BLOCK, LOOP, END, HALT, SEND, RECV};

typedef struct primitive {
    int ticks;
    int type;
} primitive;

typedef struct stackcontent {
    int nextLine;
    int iterationsleft;
} stackcontent;

typedef struct process {
    int stackindex;
    int pid;
    stackcontent* stackloop;
    primitive* primitiveslist;
    int state;
    int duration;
    int waittime;
    int waitcount;
    int blocktime;
    int doopcount;
    int blockcount;
    int enqueuetime;
    int finishtime;
    int runtime;
    int prioritiy;
    int size;
    int index;
    int address;
    int node_id;
    int send_count;             //total send count
    int receive_count;          //total receive count
    int waiting;                //waiting flag
    int time_done;              //time for when a process is done communication
} process;


extern process** procs;
extern unsigned int numberOfProcesses, quantum;

void printProcess(process* p, thread_node * node);
int readInput();
void parseprocess(process* p);

process* initializeprocess();
void getnextprimitive(process* p);

void stackpush(stackcontent data, stackcontent* stack, int* stackindex, int stacksize);
stackcontent stackpop(int* stackindex, stackcontent * stack);

#endif //OS_FINAL_PROCESS_H

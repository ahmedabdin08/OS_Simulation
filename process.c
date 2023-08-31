//
// Created by Ahmed Khairallah
//

#include "process.h"
#include "scheduler.h"
#include "node.h"
#include <stdio.h>
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;


unsigned int numberOfProcesses = 0, quantum = 0;
unsigned int numberOfNodes = 0, * ids =  NULL;

process** procs;

char* processState[] = { "new", "ready", "running", "blocked", "finished" };
enum{FALSE, TRUE};
void stackpush(stackcontent data, stackcontent* stack, int* stackindex, int stacksize) {
    if (*stackindex > stacksize) printf("Stack Overflow");
    (*stackindex)++;
    stack[*stackindex] = data;
}

stackcontent stackpop(int* stackindex, stackcontent * stack) {
    if (*(stackindex) < 0) printf("Stack Underflow");
    (*stackindex)--;
    return stack[*stackindex + 1];
}

void parseprimitives(process * p, int size) {
    char buffer[MAXSTRLEN];
    int ticks;

    p->stackloop = (stackcontent*)malloc(sizeof(stackcontent) * size);
    p->primitiveslist = (primitive*)malloc(sizeof(primitive) * size);

    if (p->primitiveslist == NULL || p->stackloop == NULL) {
        printf("Memory hasn't been properly allocated for the primitives\n");
        return;
    }

    for (int i = 0; i < size; i++) {

        if (scanf("%s", buffer) != 1) {
            printf("\nError reading primiitve\n");
        }

        if (strcmp("DOOP", buffer) == 0) {
            if (scanf("%d", &ticks) != 1) {
                printf("Error reading number after primitive\n");
            }

            (p->primitiveslist)[i].ticks = ticks;
            (p->primitiveslist)[i].type = DOOP;
        }
        else if (strcmp("SEND", buffer) == 0) {
            if (scanf("%d", &ticks) != 1) {
                printf("Error reading number after primitive\n");
            }

            (p->primitiveslist)[i].ticks = ticks;
            (p->primitiveslist)[i].type = SEND;
        }
        else if (strcmp("RECV", buffer) == 0) {
            if (scanf("%d", &ticks) != 1) {
                printf("Error reading number after primitive\n");
            }

            (p->primitiveslist)[i].ticks = ticks;
            (p->primitiveslist)[i].type = RECV;
        }
        else if (strcmp("BLOCK", buffer) == 0) {
            if (scanf("%d", &ticks) != 1) {
                printf("Error reading number after primitive\n");
            }

            (p->primitiveslist)[i].ticks = ticks;
            (p->primitiveslist)[i].type = BLOCK;
        }
        else if (strcmp("LOOP", buffer) == 0) {
            if (scanf("%d", &ticks) != 1) {
                printf("Error reading number after primitive\n");
            }

            (p->primitiveslist)[i].ticks = ticks;
            (p->primitiveslist)[i].type = LOOP;
        }
        else if (strcmp("END", buffer) == 0) {
            (p->primitiveslist)[i].ticks = -1;
            (p->primitiveslist)[i].type = END;
        }
        else if (strcmp("HALT", buffer) == 0) {
            (p->primitiveslist)[i].ticks = -1;
            (p->primitiveslist)[i].type = HALT;
        }
        else {
            printf("Invalid primitive found");
        }

    }

    if (strcmp("HALT", buffer) != 0) {
        printf("HALT primitive is not in the last line\n");
    }
}

void getnextprimitive(process* p) {
    int count = -1;
    do {
        p->index++;
        switch (p->primitiveslist[p->index].type) {
            case LOOP: {
                stackcontent member = { p->index, p->primitiveslist[p->index].ticks};
                stackpush(member, p->stackloop, &p->stackindex, p->size);
                break;
            }
            case DOOP:
                p->runtime += p->primitiveslist[p->index].ticks;
                p->doopcount++;
                break;
            case BLOCK:
                p->blocktime += p->primitiveslist[p->index].ticks;
                p->blockcount++;
                break;
            case END:
                count = p->stackloop[p->stackindex].iterationsleft;//is essentially a peak
                count--;
                if (count == 0) {
                    stackpop(&p->stackindex, p->stackloop);
                }
                else {
                    p->index = p->stackloop[p->stackindex].nextLine;
                    p->stackloop[p->stackindex].iterationsleft = count;
                }
                break;
            case SEND:

                p->send_count++;
                break;
            case RECV:
                p->receive_count++;
                break;
            case HALT:
                break;
            default:
                printf("error, unknown opcode %d at index %d\n", p->primitiveslist[p->index].type, p->index);
                break;
        }
    } while (p->primitiveslist[p->index].type == LOOP || p->primitiveslist[p->index].type == END);
}

void parseprocess(process* p) {
    char buffer[MAXSTRLEN];
    if (scanf(" %10s %d %d %d", buffer, &p->size, &p->prioritiy, &p->node_id) != 4) {
        printf("\nMore than 3 words in first line of the process\n");
    }
    p->primitiveslist = (primitive*)malloc(p->size * sizeof(primitive));
    p->stackloop = (stackcontent*)malloc(p->size * sizeof(stackcontent));
    parseprimitives(p, p->size);
}

int readInput(){
    if(scanf("%d %d %d", &numberOfProcesses, &quantum, &numberOfNodes) != 3){
        printf("Error reading first line\n");
        return READFAIL;
    }
    procs = (process**)malloc(numberOfProcesses * sizeof(process*));
    ids = calloc(numberOfNodes, sizeof (int));
    if (procs == NULL || ids == NULL) {
        printf("heap memory allocation failed\n");
        return READFAIL;
    }
    for (int i = 0; i < numberOfProcesses; i++) {
        procs[i] = initializeprocess();
    }

    return READSUCESS;
}

void printProcess(process* p, thread_node * node) {
    pthread_mutex_lock(&print_lock);
    printf("[%.2d] %.5d: process %d %s", node->id, node->clock, p->pid, processState[p->state]);
    if(p->primitiveslist[p->index].type == SEND)
        printf(" (send)");
    else if(p->primitiveslist[p->index].type == RECV)
        printf(" (recv)");
    printf("\n");
    pthread_mutex_unlock(&print_lock);
}

process * initializeprocess() {
    process* p;
    p = (process*)malloc(sizeof(process));
    p->duration = 0;
    p->index = -1;
    p->stackindex = -1;
    p->state = NEW;
    p->waittime = 0;
    p->blocktime = 0;
    p->runtime = 0;
    p->prioritiy = 0;
    p->finishtime = 0;
    p->waitcount = 0;
    p->enqueuetime = 0;
    p->size = 0;
    p->send_count = 0;
    p->receive_count = 0;
    p->waiting = 0;
    p->time_done = 0;
    parseprocess(p);
    return p;
}
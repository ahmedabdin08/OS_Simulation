//
// Created by Ahmed Khairallah
//

#include "scheduler.h"
#include "barrier.h"
#include "communication.h"

enum {FALSE, TRUE};
static priority_queue_t * finishedq;
pthread_mutex_t finished_lock = PTHREAD_MUTEX_INITIALIZER;

void initialize_scheduling() {
    finishedq = initialize_pq();
}

void print_finished(){
    while(!is_empty(finishedq)){
        process * proc = dequeue(finishedq);
        printf("| %.5d | Proc %.2d.%.2d | Run %d, Block %d, Wait %d, Sends %d, Recvs %d\n", proc->finishtime,
               proc->node_id, proc->pid, proc->runtime, proc->blocktime, proc->waittime, proc->send_count, proc->receive_count);
    }
}

void insertinq(process* p, int next_prim, thread_node * node) {
    if (next_prim) {
        getnextprimitive(p);
        p->duration = p->primitiveslist[p->index].ticks;
    }

    switch (p->primitiveslist[p->index].type) {
        case DOOP:
        case SEND:
        case RECV:
            p->state = READY;
            p->enqueuetime = node->clock;
            enqueue(node->readyq, p, actualpriority(p));
            p->waitcount++;
            break;
        case BLOCK:
            p->state = BLOCKED;
            p->duration += node->clock;
            enqueue(node->blockedq, p, p->duration);
            break;
        case HALT:
            p->finishtime = node->clock;
            p->state = FINISHED;
            pthread_mutex_lock(&finished_lock);
            enqueue(finishedq, p,
                    actualpriority(p));
            pthread_mutex_unlock(&finished_lock);
            break;
    }
    printProcess(p, node);
}
int actualpriority(process* p) {
    if(p->state == FINISHED){
        return p->finishtime * 100 * 100 + p->node_id * 100 + p->pid;
    }
    else if (p->prioritiy < 0) {
        return p->duration;
    }
    return p->prioritiy;
}
void scheduler(thread_node * node) {
    process* runningProcess = NULL;
    priority_queue_t * doneq = initialize_pq();
    int cpu_quantum = quantum, isFinised = is_empty(node->readyq) && is_empty(node->blockedq)
            && runningProcess == NULL && is_communicating(node);
    while (!isFinised) {
        int preempt = FALSE;

        //Step 1 unblock any processes that are ready to be unblocked
        while(!is_empty(doneq)){
            process * done = dequeue(doneq);
            done->time_done = 0;
            insertinq(done, TRUE, node);
        }

        while (!is_empty(node->blockedq)) {
            process* unblocked = peek(node->blockedq);
            if (unblocked->duration > node->clock) {
                break;
            }

            dequeue(node->blockedq);
            insertinq(unblocked, TRUE, node);
            //set the  premption flag if the priority of the unblocked process is less than the running process
            preempt |= runningProcess != NULL && unblocked->state == READY && actualpriority(runningProcess) > actualpriority(unblocked);
        }

        //Step 2 update running process
        if (runningProcess != NULL) {
            runningProcess->duration--;
            cpu_quantum--;


            if (runningProcess->primitiveslist[runningProcess->index].type == SEND){
                runningProcess->state = BLOCKED;
                printProcess(runningProcess, node);
                //ticks are the receiver address
                send(runningProcess->primitiveslist[runningProcess->index].ticks, node, runningProcess);
                runningProcess->runtime++;
                runningProcess = NULL;
            }
            else if(runningProcess->primitiveslist[runningProcess->index].type == RECV){
                runningProcess->state = BLOCKED;
                printProcess(runningProcess, node);
                //ticks are the sender address
                receive(runningProcess->primitiveslist[runningProcess->index].ticks, node, runningProcess);
                runningProcess->runtime++;
                runningProcess = NULL;
            }
            //a process stops running if it is being preempted the cpu quantum has completed or if it finished its doop primitive
            else if (runningProcess->duration == 0 || cpu_quantum == 0 || preempt) {
                insertinq(runningProcess, runningProcess->duration == 0, node);
                runningProcess = NULL;
            }
        }

        //Step 3 select next process to run if running process is NULL
        if (runningProcess == NULL && !is_empty(node->readyq)) {
            runningProcess = dequeue(node->readyq);
            runningProcess->waittime += node->clock - runningProcess->enqueuetime;
            cpu_quantum = quantum;
            runningProcess->state = RUNNING;
            printProcess(runningProcess, node);
        }
        barrier_wait();
        node->clock++;
        doneq = done_list(node);
        isFinised = is_empty(node->readyq) && is_empty(node->blockedq)
                && runningProcess == NULL && !is_communicating(node);
    }
    barrier_done();
}
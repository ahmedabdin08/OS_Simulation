//
// Created by Ahmed Khairallah
//

#ifndef OS_FINAL_BARRIER_H
#define OS_FINAL_BARRIER_H
#include "pthread.h"
typedef struct Barrier{
    pthread_mutex_t lock;   //barrier lock
    pthread_cond_t cond[2]; //array of two condition variables
    int max_threads;        //the maximum amount of threads for the barrier
    int cur_threads;        //the current number of threads in the barrier
    int active_cond;        //the active condition running in the barrier aka the index for the array
} Barrier;

extern Barrier * barrier;
//initialize the barrier
extern void barrier_init(unsigned int max);
//function so threads wait
extern void barrier_wait();
//function for when threads no longer need to use the barrier
extern void barrier_done();
#endif //OS_FINAL_BARRIER_H

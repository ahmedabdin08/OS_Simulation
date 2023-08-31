//
// Created by Ahmed Khairallah
//

#include "barrier.h"
#include "stdlib.h"

Barrier * barrier = NULL;
//initialize the barrier
extern void barrier_init(unsigned int max){
    barrier = (Barrier *)malloc(sizeof (Barrier));
    barrier->active_cond = 0;
    barrier->max_threads = max;
    barrier->cur_threads = 0;
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->cond[0], NULL);
    pthread_cond_init(&barrier->cond[1], NULL);
}
//function so threads wait
extern void barrier_wait(){
    pthread_mutex_lock(&barrier->lock);
    barrier->cur_threads++;
    if(barrier->cur_threads < barrier->max_threads){
        pthread_cond_wait(&barrier->cond[barrier->active_cond], &barrier->lock);
    }
    else{
        barrier->active_cond = !barrier->active_cond;
        barrier->cur_threads = 0;
    }
    pthread_cond_signal(&barrier->cond[!barrier->active_cond]);
    pthread_mutex_unlock(&barrier->lock);
}
//function for when threads no longer need to use the barrier
extern void barrier_done(){
    pthread_mutex_lock(&barrier->lock);
    barrier->max_threads--;
    if(barrier->cur_threads == barrier->max_threads){
        barrier->cur_threads = 0;
        barrier->active_cond = !barrier->active_cond;
        pthread_cond_signal(&barrier->cond[!barrier->active_cond]);
    }
    pthread_mutex_unlock(&barrier->lock);
}
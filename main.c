//
// Created by Ahmed Khairallah
//

#include "node.h"
#include "process.h"
#include "scheduler.h"
#include "barrier.h"
#define MAXTHREADS 100

int main(){
    initialize_scheduling();
    pthread_t tid[MAXTHREADS];
    if(readInput() == READFAIL){
        return -1;
    }
    barrier_init(numberOfNodes);
    for(int i = 0; i <numberOfNodes; i++){
        ids[i] = i +1;
        pthread_create(&tid[i], NULL, execute_thread, &ids[i]);
    }

    for(int i = 0; i < numberOfNodes; i++){
        pthread_join(tid[i], NULL);
    }

    print_finished();
    return 0;
}

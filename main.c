//
// Created by Ahmed Khairallah
//

#include "node.h"
#include "process.h"
#include "scheduler.h"
#include "barrier.h"
#include "communication.h"
#define MAXTHREADS 100

int main(){
    initialize_scheduling();
    pthread_t tid[MAXTHREADS];
    if(readInput() == READFAIL){
        return -1;
    }
    barrier_init(numberOfNodes);
    communication_locks = calloc(numberOfProcesses, sizeof (pthread_mutex_t));
    for(int i = 0; i <numberOfNodes; i++){
        ids[i] = i +1;
        pthread_create(&tid[i], NULL, execute_thread, &ids[i]);
    }

    for(int i = 0; i < numberOfNodes; i++){
        pthread_join(tid[i], NULL);
    }

    print_finished();

    //cleaning up code
    barrier_destroy();
    pthread_mutex_destroy(&print_lock);
    pthread_mutex_destroy(&finished_lock);
    for(int i = 0; i < numberOfProcesses; i++){
        pthread_mutex_destroy(&communication_locks[i]);
        free(procs[i]);
    }
    free(procs);
    free(communication_locks);

    return 0;
}

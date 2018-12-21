#include "sharedqueue.h"
#include "Queue.h"

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

pthread_t threadID_1,threadID_2,threadID_3;
data_t initdata = {1, "Shared Queue"};
data_t threadData_1 = {2 , "Producer 1"};
data_t threadData_2 = {3 , "Producer 2"};
data_t threadData_3 = {4 , "Producer 3"};
queue_t sharedQueue = {NULL}; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main (void) {
    printf("SHARED QUEUE \n\n");
    createQueue(&sharedQueue, initdata);
    
    

    pthread_create(&threadID_1, NULL, threadFunction, (void*) &threadData_1);
    pthread_create(&threadID_2, NULL, threadFunction, (void*) &threadData_2);
    pthread_create(&threadID_3, NULL, threadFunction, (void*) &threadData_3);

    sleep(15);
    
    signalStopHandler();
    showQueue(&sharedQueue);
    deleteQueue(&sharedQueue);
    printf("SHARED QUEUE END\n\n");
}



int signalStopHandler(void) {
    pthread_join(threadID_1, NULL);
    pthread_join(threadID_2, NULL);
    pthread_join(threadID_3, NULL);
  
}

void *threadFunction (void *argv) {
    int counter = 0;
    data_t *threadMessage = argv;
    printf("%s\n",threadMessage->text);
    pthread_mutex_lock(&mutex);
    pushQueue(&sharedQueue, *threadMessage);
    pthread_mutex_unlock(&mutex);
    counter ++;
    sleep(threadMessage->intVal);
    
    pthread_exit(NULL);
}
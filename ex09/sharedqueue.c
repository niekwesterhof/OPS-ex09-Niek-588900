#include "sharedqueue.h"
#include "Queue.h"

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

pthread_t threadID_1,threadID_2,threadID_3;
data_t initdata = {1, "Shared Queue"};
data_t threadData_1 = {2 , "Producer 1 produced a product"};
data_t threadData_2 = {3 , "Producer 2 produced a product"};
data_t threadData_3 = {4 , "Producer 3 produced a product"};
queue_t sharedQueue = {NULL}; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool writeQueue = true;

int main (void) {
    struct sigaction act, oldact;
    printf("SHARED QUEUE \n\n");
    createQueue(&sharedQueue, initdata);
    // define SHR
    memset(&act, '\0', sizeof(act));
    act.sa_handler = signalStopHandler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    // install SHR
    sigaction(SIGINT, &act,&oldact);
    // create 3 thread for the 3 producers
    pthread_create(&threadID_1, NULL, threadFunction, (void*) &threadData_1);
    pthread_create(&threadID_2, NULL, threadFunction, (void*) &threadData_2);
    pthread_create(&threadID_3, NULL, threadFunction, (void*) &threadData_3);

    sleep(15);
    
    exitThreads();
    showQueue(&sharedQueue);
    deleteQueue(&sharedQueue);
    sigaction(SIGINT, &oldact, NULL);
    printf("SHARED QUEUE END\n\n");
}



void exitThreads(void) {
    writeQueue = false;
    pthread_join(threadID_1, NULL);
    pthread_join(threadID_2, NULL);
    pthread_join(threadID_3, NULL);
  
}

void *threadFunction (void *argv) {

    data_t *threadMessage = argv;
    int delay = threadMessage->intVal;
    threadMessage->intVal = 0;
    printf("%s\n",threadMessage->text);
    while (writeQueue)
    {
        pthread_mutex_lock(&mutex);
        pushQueue(&sharedQueue, *threadMessage);
        pthread_mutex_unlock(&mutex);
        threadMessage->intVal ++;
        sleep(threadMessage->intVal);
    }

    pthread_exit(NULL);
}

int signalStopHandler(int signal) {
    writeQueue = false;
    return 0;
}
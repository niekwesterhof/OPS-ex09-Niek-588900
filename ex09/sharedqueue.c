#include "sharedqueue.h"
#include "Queue.h"

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>


pthread_t threadID_1,threadID_2,threadID_3, threadID_4;
data_t initdata = {1, "Shared Queue"};
data_t threadData_1 = {2 , "Producer 1 produced a product"};
data_t threadData_2 = {3 , "Producer 2 produced a product"};
data_t threadData_3 = {4 , "Producer 3 produced a product"};
data_t threadData_4 = {15 , "Consumer eats all products"};
queue_t sharedQueue = {NULL}; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool writeQueue = true;
bool running = true;


int main (void) {
    struct sigaction act, oldact;
    printf("\nPROGRAM -- SHARED QUEUE -- STARTED \n\n");
    // initialisation of the Queue
    emptyFile();
    createQueue(&sharedQueue, initdata);

    // define SHR
    memset(&act, '\0', sizeof(act));
    act.sa_handler = signalStopHandler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    // install SHR
    sigaction(SIGINT, &act, &oldact);

    // create 3 thread for the 3 producers
    pthread_create(&threadID_1, NULL, threadProducer, (void*) &threadData_1);
    pthread_create(&threadID_2, NULL, threadProducer, (void*) &threadData_2);
    pthread_create(&threadID_3, NULL, threadProducer, (void*) &threadData_3);
    pthread_create(&threadID_4, NULL, threadConsumer, (void*) &threadData_4);
    
    //waits for threads to finish
    pthread_join(threadID_1, NULL);
    pthread_join(threadID_2, NULL);
    pthread_join(threadID_3, NULL);
    pthread_join(threadID_4, NULL);

    // restore SHR
    sigaction(SIGINT, &oldact, NULL);
    
    // delete the shared SSL
    deleteQueue(&sharedQueue);
    printf("\n\nPROGRAM -- SHARED QUEUE -- END\n\n");
}



void exitThreads(void) {
    running = false;
}

void *threadProducer (void *argv) {

    data_t *threadMessage = argv;
    int delay = threadMessage->intVal;
    threadMessage->intVal = 0;
    
    while (writeQueue)
    {
        pthread_mutex_lock(&mutex);
        pushQueue(&sharedQueue, *threadMessage);
        pthread_mutex_unlock(&mutex);
        threadMessage->intVal ++;
        sleep(delay);
    }
    
    pthread_exit(NULL);
}

void *threadConsumer (void *argv) {

    data_t *threadMessage = argv;
    int delay = threadMessage->intVal;
    while (running)
    {
        // sleep for 15 seconds
        sleep(delay);
        //show the filled shared SLL and delete SLL
        pthread_mutex_lock(&mutex);
        showQueue(&sharedQueue);
        deleteQueue(&sharedQueue);
        createQueue(&sharedQueue, initdata);
        pthread_mutex_unlock(&mutex);
    }
}

void signalStopHandler(int signal) {
    if (signal == SIGINT)
    {
         writeQueue = false;
        exitThreads();
    }
}

void emptyFile(void) {
  FILE* pfile = fopen("ex09.txt","w");
    if(pfile == NULL)
    {
      printf("error opeing file\n");
      exit(EXIT_FAILURE);
    }
    fprintf(pfile,"-- SHARED RESOURCE --\n NIEK WESTERHOF 588900\n\n");
    pclose(pfile);
}
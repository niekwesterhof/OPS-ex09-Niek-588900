#ifndef SHAREDQUEUE_H
#define SHAREDQUEUE_H

#include <stdlib.h>
#define TEXT_SIZE 30

typedef struct {
  int delay;
  int counter;
  char text[TEXT_SIZE];
} threadData_t;

// Prototype functions
int createThread(int time, const char message [20]);
int signalStopHandler(int signal);
void *threadFunction(void* argv);
void exitThreads(void);

#endif
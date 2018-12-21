/******************************************************************************
 * File:         Queue.c
 * Version:      1.2
 * Date:         2017-10-18
 * Author:       J. Onokiewicz, M. van der Sluys
 * Description:  OPS exercise 5: Queues
 ******************************************************************************/


#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new queue:
void createQueue(queue_t* queue, data_t data) {
  if(queue->lastNode != NULL) deleteQueue(queue);
  
  node_t* newNode = (node_t*) malloc(sizeof(node_t));
  queue->lastNode = newNode;
  if(newNode != NULL) {
    newNode->data = data;
    newNode->nextNode = newNode;
  }
}


// Remove all data from a queue:
int emptyQueue(const queue_t* queue) {
  return (queue->lastNode == NULL);
}


// Return the size of a queue:
size_t sizeQueue(const queue_t* queue) {
  size_t size = 1;
  node_t* currentNode;
    
  // Special case: empty queue
  if(emptyQueue(queue)) return 0;
  
  // Loop until circle is complete (nextNode equals last node):
  for(currentNode = queue->lastNode;
      currentNode != NULL && currentNode->nextNode != queue->lastNode;
      currentNode = currentNode->nextNode) {
    size++;
  }
  
  return size;
}


// Return the first node of a queue:
data_t* frontQueue(const queue_t* queue) {
  return &(queue->lastNode->nextNode->data);
}


// Return the last node of a queue:
data_t* backQueue(const queue_t* queue) {
  return &(queue->lastNode->data);
}


// Producer: add a node (=data) to a queue:
void pushQueue(queue_t* queue, data_t data) {
  node_t* newNode = (node_t*) malloc(sizeof(node_t));
  if(newNode != NULL) {
    newNode->data = data;                               // Set data
    newNode->nextNode = queue->lastNode->nextNode;  // New last node points to first node
    queue->lastNode->nextNode = newNode;             // Old last node points to new last node
    queue->lastNode = newNode;                        // Set new last-node pointer
  }
}


// Consumer: remove data from a queue:
void popQueue(queue_t* queue) {
  if(queue->lastNode != NULL) {
    node_t* pDelete = queue->lastNode->nextNode;
    if(pDelete == queue->lastNode) {  // Case n=1: make queue empty:
      queue->lastNode = NULL;
    } else {                          // Case n>1: point lastNode to next first node:
      queue->lastNode->nextNode = pDelete->nextNode;
    }
    free(pDelete);
  }
}


// Delete a queue and its contents:
void deleteQueue(queue_t* queue) {
  node_t* currentNode = queue->lastNode, * nextN = NULL;
  
  do {
    if(currentNode == NULL) break;
    nextN = currentNode->nextNode;
    free(currentNode);
    currentNode = nextN;
  } while(currentNode != queue->lastNode);
  
  queue->lastNode = NULL;
}


// List the data/nodes in a queue:
void showQueue(const queue_t* queue) {
  const node_t* nextN = queue->lastNode;
  if(nextN == NULL) {
    printf("Queue is empty, last node is %p\n", (void*) queue->lastNode);
  } else {
    printf("Queue contains %ld nodes:\n", sizeQueue(queue));
    printf("Last node: %p\n", (void*) queue->lastNode);
    do {
      nextN = nextN->nextNode;
      printf("pNode = %p  Data = '%d' '%s'  nextN = %p\n",
             (void*)nextN, nextN->data.intVal, nextN->data.text, (void*)nextN->nextNode);
    } while(nextN != queue->lastNode);
  }
}

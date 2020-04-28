
#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>


#define condition ;



typedef pthread_mutex_t Mutex;
typedef pthread_cond_t Cond;
typedef sem_t Semaphore;
typedef struct
{
    char *array;
    int length;
    int lengthProducer;
    int lengthConsumer;
    int next_in;
    int next_out;
    int next_inconsumerQueue;
    int next_outconsumerQueue;
    int next_inproducerQueue;
    int next_outproducerQueue;
    Mutex *fifo;
    Mutex *consumerQueue;
    Mutex *producerQueue;
#ifdef condition
    Cond *nonEmpty;
    Cond *nonFull;
    Cond *nonEmptyQueueConsumer;
    Cond *nonFullQueueConsumer;
    Cond *nonEmptyQueueProducer;
    Cond *nonFullQueueProducer;
#else
    Semaphore *items;
    Semaphore *spaces;
    Semaphore *itemsQueueProducer;
    Semaphore *spacesQueueProducer;
    Semaphore *itemsQueueConsumer;
    Semaphore *spacesQueueConsumer;
#endif // semaphor
} FIFOStack;

typedef struct
{
    pthread_t thread;
    Mutex *pause;
    int flag;
} CPThread;

extern CPThread *producerThreadOne;
extern CPThread *producerThreadTwo;
extern CPThread *consumerThreadOne;
extern pthread_t threadControl;

void *check_malloc(int size);
void cancelEnable();
void cancelDisable();

#endif
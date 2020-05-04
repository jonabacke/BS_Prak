
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
#include <mqueue.h>


#define condition 
#define sizeConsumerQueue 10 
#define sizeProducerQueue 10 




typedef pthread_mutex_t Mutex;
typedef pthread_cond_t Cond;
typedef sem_t Semaphore;
typedef struct
{
    char *array;
    int length;
    int next_in;
    int next_out;
    Mutex *fifo;
#ifdef condition
    Cond *nonEmpty;
    Cond *nonFull;
#else
    Semaphore *items;
    Semaphore *spaces;
#endif // semaphor
} FIFOStack;

typedef struct 
{
    mqd_t schlange;
    Mutex *queue;
    int next_in;
    int next_out;
    int length;
#ifdef condition
    Cond *nonEmpty;
    Cond *nonFull;
#else
    Semaphore *items;
    Semaphore *spaces;
#endif // semaphor
} QueueStruct;

typedef struct
{
    pthread_t thread;
    FIFOStack *stack;
    int flag;
    char value;
    QueueStruct *queue;

} CPThread;




extern Mutex *generateConsumerMutex;
extern Mutex *generateProducerMutex;

extern pthread_t threadControl;
extern pthread_t producerQueue;
extern pthread_t consumerQueue;

void *check_malloc(int size);
void cancelEnable();
void cancelDisable();

#endif
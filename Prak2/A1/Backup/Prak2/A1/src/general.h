
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


#define condition
#define sleepConsumerTimer  3
#define sleepProducerTimer  2
#define TURN_ON				1
#define TURN_OFF			0



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
    pthread_t thread;
    Mutex *pause;
    int flag;
    char *alphabet;
    char *name;
    FIFOStack *stack;
} CPThread;

extern CPThread *producerThreadOne;
extern CPThread *producerThreadTwo;
extern CPThread *consumerThreadOne;
extern pthread_t threadControl;

void *check_malloc(int size);
void cancelEnable();
void cancelDisable();



#endif

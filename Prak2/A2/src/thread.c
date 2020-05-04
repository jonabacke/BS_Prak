#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "thread.h"


void make_thread(pthread_t *thread, void *funktion, CPThread *t)
{
    int n = pthread_create(thread, NULL, funktion, t);
    if (n != 0)
    {
        perror("make_thread failed");
        exit(EXIT_FAILURE);
    }
}

CPThread *makeConsumerProducerThread(void *funktion, FIFOStack *stack, int length, QueueStruct *queue)
{
    CPThread *thread = check_malloc(sizeof(CPThread));
    make_thread(&(thread->thread), funktion, thread);
    thread->flag = 1;
    thread->stack = stack;
    thread->value = ' ';
    thread->queue = queue;
    
    return thread;    
}


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "thread.h"


void make_thread(pthread_t *thread, void *funktion, void *stack)
{
    int n = pthread_create(thread, NULL, funktion, stack);
    if (n != 0)
    {
        perror("make_thread failed");
        exit(EXIT_FAILURE);
    }
}

CPThread *makeConsumerProducerThread(void *funktion, FIFOStack *stack)
{
    CPThread *thread = check_malloc(sizeof(CPThread));
    make_thread(&(thread->thread), funktion, stack);
    thread->pause = make_mutex();
    thread->flag = 1;
    return thread;    
}


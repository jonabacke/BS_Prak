#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "general.h"

CPThread *producerThreadOne = NULL;
CPThread *producerThreadTwo = NULL;
CPThread *consumerThreadOne = NULL;

void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    return p;
}

void cancelEnable()
{
    int scs = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (scs != 0)
    {
        perror("pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }
}

void cancelDisable()
{
    int scs = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if (scs != 0)
    {
        perror("pthread_setcancelstate failed");
        exit(EXIT_FAILURE);
    }
}
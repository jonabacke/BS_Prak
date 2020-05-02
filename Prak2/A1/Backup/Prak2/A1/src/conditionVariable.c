#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "conditionVariable.h"


void cond_wait(Cond *cond, Mutex *mutex)
{
    int n = pthread_cond_wait(cond, mutex);
    if (n != 0)
    {
        perror("cond_wait failed");
        exit(EXIT_FAILURE);
    }
}

void cond_signal(Cond *cond)
{
    int n = pthread_cond_signal(cond);
    if (n != 0)
    {
        perror("cond_wait failed");
        exit(EXIT_FAILURE);
    }
}

Cond *make_cond(void)
{
    Cond *cond = check_malloc(sizeof(Cond));
    int n = pthread_cond_init(cond, NULL);
    if (n != 0)
    {
        perror("make_cond failed");
        exit(EXIT_FAILURE);
    }
    return cond;
}


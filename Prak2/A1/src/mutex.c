#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>


#include "mutex.h"


Mutex *make_mutex(void)
{
    Mutex *mutex = check_malloc(sizeof(Mutex));
    int n = pthread_mutex_init(mutex, NULL);
    if (n != 0)
    {
        perror("make_lock failed");
        exit(EXIT_FAILURE);
    }
    return mutex;
}


void mutex_lock(Mutex *mutex)
{
    int n = pthread_mutex_lock(mutex);
    if (n != 0)
    {
        perror("lock failed");
        exit(EXIT_FAILURE);
    }
}
void mutex_unlock(Mutex *mutex)
{
    int n = pthread_mutex_unlock(mutex);
    if (n != 0)
    {
        perror("lock failed");
        exit(EXIT_FAILURE);
    }
}

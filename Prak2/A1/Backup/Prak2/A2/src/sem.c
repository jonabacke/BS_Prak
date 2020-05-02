#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "sem.h"

Semaphore *make_semaphore(int value)
{
    Semaphore *sem = check_malloc(sizeof(Semaphore));
    int n = sem_init(sem, 0, value);
    if (n != 0)
    {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }
    return sem;
}

void semaphore_wait(Semaphore *sem)
{
    int n = sem_wait(sem);
    if (n != 0)
    {
        perror("sem_wait failed");
        exit(EXIT_FAILURE);
    }
}

void semaphore_post(Semaphore *sem)
{
    int n = sem_post(sem);
    if (n != 0)
    {
        perror("sem_ost failed");
        exit(EXIT_FAILURE);
    }
}

#ifndef SEM_H
#define SEM_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "general.h"


typedef sem_t Semaphore;


Semaphore *make_semaphore(int value);

void semaphore_wait(Semaphore *sem);

void semaphore_post(Semaphore *sem);

#endif
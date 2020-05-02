#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "general.h"
typedef pthread_cond_t Cond;
typedef pthread_mutex_t Mutex;


void cond_wait(Cond *cond, Mutex *mutex);

void cond_signal(Cond *cond);

Cond *make_cond(void);
#endif 
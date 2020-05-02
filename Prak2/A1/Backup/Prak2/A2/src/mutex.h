
#ifndef MUTEX_H
#define MUTEX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "general.h"


typedef pthread_mutex_t Mutex;


Mutex *make_mutex(void);
void mutex_lock(Mutex *mutex);
void mutex_unlock(Mutex *mutex);

#endif
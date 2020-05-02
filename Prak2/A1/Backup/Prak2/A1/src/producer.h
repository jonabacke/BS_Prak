
#ifndef PRODUCER_H
#define PRODUCER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "general.h"
#include "sem.h"
#include "conditionVariable.h"
#include "mutex.h"
#include "fifo.h"




void *producerHandler(CPThread *stack);

void producer(FIFOStack *stack, char value, CPThread *thread);

void unlockMutex(void *arg);

#endif


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
#include "taskQueue.h"




void *producerOne(void *stack);

void *producerTwo(void *stack);

void producer(FIFOStack *stack);

#endif
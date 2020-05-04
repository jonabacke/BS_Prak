#ifndef FIFO_H
#define FIFO_H

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
#include "mutex.h"
#include "conditionVariable.h"


typedef sem_t Semaphore;

FIFOStack *make_stack(int length);


int stack_incr(int length, int next);


int stack_empty(FIFOStack *stack);


int stack_full(FIFOStack *stack);

int queue_full(QueueStruct *queue);

#endif
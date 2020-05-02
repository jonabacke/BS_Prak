#ifndef CONSUMER_H
#define CONSUMER_H

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
#include "fifo.h"



void consumer(FIFOStack *stack);


void *consumerOne(void *stack);



#endif
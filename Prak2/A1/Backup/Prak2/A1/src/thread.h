
#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "general.h"
#include "mutex.h"
#include "consumer.h"
#include "producer.h"




void make_thread(pthread_t* thread, void *funktion, void *stack);

CPThread *makeConsumerProducerThread(void *funktion, FIFOStack *stack, char *name, char *alphabet);

#endif
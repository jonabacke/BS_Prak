#ifndef CONTROLER_H
#define CONTROLER_H

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


void *control(void *not_used);

void toggleThread(CPThread *stack);

void printCommands();

void cancelAll();

#endif

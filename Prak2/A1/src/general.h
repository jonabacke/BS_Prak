/* ============================================================================
 * @file        : general.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides shared includes, typedefs, macros etc.
 * ============================================================================
 */


#ifndef GENERAL_H
#define GENERAL_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "mutex.h"
#include "conditionVariable.h"
#include "thread.h"


#define condition
#define FOR_3_SECONDS       3
#define FOR_2_SECONDS       2
#define TURN_ON				1
#define TURN_OFF			0
#define BUFFER_SIZE         10


// This macro prints an error gererated by the command cmd.
// Standard Unix error handling and posix error handling will be supported.
#define HANDLE_ERR(cmd) \
{ \
    int e = cmd; \
    if (e) \
    { \
       printf( "Error in line %d of file %s: %s\n", __LINE__, __FILE__, strerror((e==-1)?errno:e)); \
       exit(1); \
    } \
}


/*TYPEDEFS*/
typedef pthread_mutex_t Mutex;
typedef pthread_cond_t Cond;
typedef sem_t Semaphore;

/*Struct for the FIFO Buffer*/
typedef struct
{
    char bufferContent[BUFFER_SIZE];
    uint8_t readPointer;
    uint8_t writePointer;
    uint8_t bufferLevel;
    Mutex *fifoMutex;
#ifdef condition
    Cond *buffer_empty;
    Cond *buffer_not_empty;
    Cond *buffer_full;
    Cond *buffer_not_full;
#else
    Semaphore *buffer_elements;
    Semaphore *buffer_capacity;
#endif // semaphor
} FIFOBuffer;


/*Struct for a thread*/
typedef struct
{
    pthread_t thread;
    Mutex *pause;
    int flag;
    char *alphabet;
    char *name;
    FIFOBuffer *fifoBuffer;
} CPThread;


extern CPThread *Producer_1;
extern CPThread *Producer_2;
extern CPThread *Consumer;
extern pthread_t threadControl;


void *check_malloc(int size);
void cancelEnable();
void cancelDisable();


#endif /*_GENERAL_H*/

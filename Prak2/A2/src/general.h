/* ============================================================================
 * @file        : general.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides shared includes, typedefs, macros etc.
 * ============================================================================
 */


#ifndef _GENERAL_H
#define _GENERAL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "taskQueue.h"


#define conditio                  0
#define BUFFER_SIZE                 10
#define THREE_SECONDS               3
#define TWO_SECONDS                 2
#define TURN_ON				        1
#define TURN_OFF			        0
#define SYSTEM_ACTIVE               1
#define CONSUMER_THREAD_ACTIVE      1
#define PRODUCER_THREAD_ACTIVE      1
#define QUEUE_SIZE                  10


// This marco prints an error gererated by the command cmd.
// Standard Unix error handling and posix error handling will be supported.
#define HANDLE_ERR(cmd) \
{ int e = cmd; \
    if (e) { \
       printf( "Error in line %d of file %s: %s\n", __LINE__, __FILE__, strerror((e==-1)?errno:e)); \
       exit(1); \
    } \
}



/*TYPEDEFS*/

typedef pthread_mutex_t Mutex;

typedef pthread_cond_t Cond;

typedef sem_t Semaphore;

typedef struct  /*FIFO BUFFER*/
{
    char *bufferContent;
    int32_t readPointer;
    int32_t writePointer;
    int32_t bufferLevel;
    Mutex *bufferMutex;
#ifdef condition /*CONDITION VARIABLES*/
    Cond *buffer_not_empty;
    Cond *buffer_not_full;
#else /*SEMAPHORES*/
    Semaphore *buffer_elements;
    Semaphore *buffer_capacity;
#endif 
} FIFOBuffer;


typedef struct  /*Queue*/
{
    int32_t readPointer;
    int32_t writePointer;
    int32_t length;
    Mutex *bufferMutex;
    mqd_t queue;
    struct TaskHeader *header;
    Mutex *block;
    int flag;
#ifdef condition /*CONDITION VARIABLES*/
    Cond *buffer_not_empty;
    Cond *buffer_not_full;
#else /*SEMAPHORES*/
    Semaphore *buffer_elements;
    Semaphore *buffer_capacity;
#endif 
} Queue;


//typedef struct /*THREAD*/
//{
//    pthread_t thread;
//    FIFOBuffer *fifoBuffer;
//    Queue *queue;
//    u_int8_t flag;
//} CPThread;









#endif

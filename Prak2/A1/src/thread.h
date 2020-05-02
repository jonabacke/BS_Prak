/* ============================================================================
 * Name        : thread.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides a function to create producer or consumer threads.
 * ============================================================================
 */


#ifndef _THREAD_H
#define _THREAD_H


#include "general.h"
#include "mutex.h"
#include "thread.h"
#include "fifo.h"
#include "producer.h"
#include "consumer.h"


/* @brief   Initializes a producer or a consumer thread.
*  @param   fifoBuffer  The buffer to read from or write on.
*  @param   name        Name of the Thread
*/
CPThread *makeConsumerProducerThread(FIFOBuffer *fifoBuffer, char *name);


#endif /*_THREAD_H*/
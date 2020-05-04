/* ============================================================================
 * Name        : consumer.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the consume function for the consumer thread.
 * ============================================================================
 */


#ifndef _CONSUMER_H
#define _CONSUMER_H



#include "general.h"
#include "queue.h"



/* ============================================================================
*  @brief   The Consumer Handler keeps the Consumer thread using the 'consumer'
*           function to read a letter from the FIFO buffer, print the letter 
*           and then pause for 3 seconds.
*  @param   thread      Consumer thread
*/
void *consumerHandler(CPThread *thread);



/* @brief   A Consumer thread uses this function to take a letter from the 
*           FIFO buffer and print it. After reading the letter the thread takes 
*           a break for 3 seconds.
*  @param   fifoBuffer  The buffer to read the letter from.
*  @param   thread      Consumer
*/
char consumer(FIFOBuffer *fifoBuffer);



//void unlockMutex(void *arg);



#endif /*_CONSUMER_H*/

/* ============================================================================
 * Name        : producer.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the produce function for the producer threads.
 * ============================================================================
 */


#ifndef _PRODUCER_H
#define _PRODUCER_H


#include <ctype.h>
#include "general.h"
#include "fifo.h"


/* @brief   A Producer thread uses this function to put a letter from the alphabet into the FIFO buffer.
*           Producer_1 submits a lower case letter and Producer_2 an upper case letter.
*           After submitting the letter the thread takes a break for 2 seconds.
*  @param   fifoBuffer  The buffer to put the letter into.
*  @param   thread      Producer_1 or Producer_2
*/
void *produce(FIFOBuffer *fifoBuffer, CPThread *thread);


#endif /*_PRODUCER_H*/




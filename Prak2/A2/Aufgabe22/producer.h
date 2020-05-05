/* ============================================================================
 * Name        : producer.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the produce function for the producer threads.
 * ============================================================================
 */


#ifndef PRODUCER_H
#define PRODUCER_H

#include <ctype.h> //tolower
#include "fifo.h"



/* ============================================================================
*  @brief   The Producer Handler keeps the Producer threads using the 'producer'
*           function to write a letter from the alphabet to the FIFO buffer, 
*           print the letter and then pause for 2 seconds.
*  @param   'thread'    Producer thread
*/
void *producerHandler(CPThread *thread);



/* ============================================================================
*  @brief   A Producer thread uses this function to write a letter into the FIFO buffer.
*  @param   'fifoBuffer'    The buffer to put the letter into.
*  @param   'letter'        The letter to write
*  @param   'thread'        Producer_1 or Producer_2
*/
void producer(char letter);



#endif /*_PRODUCER_H*/

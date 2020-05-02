/* ============================================================================
 * @file        : fifo.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the FIFO buffer with it's functionalities
 * ============================================================================
 */


#ifndef _FIFO_H
#define _FIFO_H


#include "general.h"
#include "mutex.h"
#include "semaphore.h"


typedef sem_t Semaphore;


/* @brief   Initializes a FIFO buffer
*/
FIFOBuffer *make_fifoBuffer();


/* @brief   Write a letter in the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   fifoBuffer  The buffer to work on
*  @param   thread      The thread writing in the buffer
*  @param   letter      The letter to be written in the buffer
*/
void writeInFIFO(FIFOBuffer *fifoBuffer, CPThread *thread, char letter);


/* @brief   Read a letter from the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   fifoBuffer  The buffer to work on
*  @param   thread      The thread reading from the buffer
*/
char readFromFIFO(FIFOBuffer *fifoBuffer, CPThread *thread);


#endif /*_FIFO_H*/
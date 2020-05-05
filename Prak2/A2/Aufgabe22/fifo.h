/* ============================================================================
 * @file        : fifo.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the FIFO buffer with it's functionalities
 * ============================================================================
 */


#ifndef FIFO_H
#define FIFO_H


#include "sem.h"
#include "conditionVariable.h"


typedef sem_t Semaphore;



/* ============================================================================
*  @brief   Initialize a FIFO buffer.
*  @return  Returns an initalized FIFO buffer.
*/
FIFOBuffer *make_FIFOBuffer(void);



/* ============================================================================
*  @brief   Write a letter in the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   'fifoBuffer'  The buffer to work on
*  @param   'letter'      The letter to be written in the buffer
*/
void writeInFIFO(char letter);



/* ============================================================================
*  @brief   Read a letter from the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   'fifoBuffer'    The buffer to work on
*  @return  'letter'        Returns the letter that has been read from the FIFO buffer.
*/
void readFromFIFO(char *letter);



/* ============================================================================
*  @brief   Increases the readPointer or the writePointer of the FIFO buffer.
*           Increases by 1 with each step until buffer size is reached.
*           Then restarts at 1.
*  @param   'fifoBuffer'    The buffer controlled via the pointers.
*  @param   'next'          Either the readPointer or the writePointer to increase.
*  @return  'next'          Returns the correctly moved pointer.
*/
int bufferPointer_incr(int next);



/* ============================================================================
*  @brief   If the writePointer and the readPointer are at the same level,
*           the FIFO buffer is empty.  
*  @param   'fifoBuffer'    The buffer to be checked.  
*  @return  'true'          if the buffer is empty else 'false'.
*/
int is_buffer_empty(FIFOBuffer *fifoBuffer);



/* ============================================================================
*  @brief   If the writePointer+1 is at the same level as the readPointer,
*           the buffer is full.
*  @param   'fifoBuffer'    The buffer to be checked.  
*  @return  'true'          if the buffer is full else 'false'.
*/
int is_buffer_full(FIFOBuffer *fifoBuffer);



#endif /*_FIFO_H*/

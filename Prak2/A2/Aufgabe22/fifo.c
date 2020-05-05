/* ============================================================================
 * @file        : fifo.c
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the FIFO buffer with it's functionalities
 * ============================================================================
 */


#include "fifo.h"

FIFOBuffer *fifoBuffer;

/* ============================================================================
*  @brief   Initialize a FIFO buffer.
*  @return  Returns an initialized FIFO buffer.
*/
FIFOBuffer *make_FIFOBuffer()
{
    fifoBuffer = (FIFOBuffer *)check_malloc(sizeof(FIFOBuffer));
    fifoBuffer->bufferContent = (char *)check_malloc(BUFFER_SIZE * sizeof(char));
    fifoBuffer->readPointer = 0;
    fifoBuffer->writePointer = 0;
    fifoBuffer->bufferLevel = 0;
    fifoBuffer->bufferMutex = make_mutex();
#ifdef condition
    // fifoBuffer->buffer_empty = make_cond();
    fifoBuffer->buffer_not_empty = make_cond();
    // fifoBuffer->buffer_full = make_cond();
    fifoBuffer->buffer_not_full = make_cond();
#else
    fifoBuffer->buffer_elements = make_semaphore(0);
    fifoBuffer->buffer_capacity = make_semaphore(BUFFER_SIZE - 1);
#endif 

    return fifoBuffer;
}



/* ============================================================================
*  @brief   Write a letter in the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   'fifoBuffer'  The buffer to work on
*  @param   'letter'      The letter to be written in the buffer
*/
void writeInFIFO(char letter)
{

#ifdef condition /*Conditional variables*/
    mutex_lock(fifoBuffer->bufferMutex);
    pthread_cleanup_push(cleanup_handler, fifoBuffer->bufferMutex);
    while (is_buffer_full(fifoBuffer))
    {
        cond_wait(fifoBuffer->buffer_not_full, fifoBuffer->bufferMutex);                		//cond wait -> buffer full
    }
	cancelDisable();

    /*CRITICAL SECTION*/
    fifoBuffer->bufferContent[fifoBuffer->writePointer] = letter;                           // write letter in buffer
    fifoBuffer->writePointer = bufferPointer_incr(fifoBuffer->writePointer);                // set writepointer
    fifoBuffer->bufferLevel ++;
    printf("BufferLevel = %d\n", fifoBuffer->bufferLevel);

    mutex_unlock(fifoBuffer->bufferMutex);
    pthread_cleanup_pop(1);
    cond_signal(fifoBuffer->buffer_not_empty);  												//cond signal -> buffer not full
    cancelEnable();

#else /*Semaphores*/
    semaphore_wait(fifoBuffer->buffer_capacity);                                            // sem buffer capacity down
    cancelDisable();
    mutex_lock(fifoBuffer->bufferMutex);

    /*CRITICAL SECTION*/
    fifoBuffer->bufferContent[fifoBuffer->writePointer] = letter;                           // write letter in buffer
    fifoBuffer->writePointer = bufferPointer_incr(fifoBuffer->writePointer);    // set writepointer
    fifoBuffer->bufferLevel ++;
    printf("BufferLevel = %d\n", fifoBuffer->bufferLevel);

    mutex_unlock(fifoBuffer->bufferMutex);
    cancelEnable();
    semaphore_post(fifoBuffer->buffer_elements);                                            // sem buffer elements up

#endif // cond

}


/* ============================================================================
*  @brief   Read a letter from the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   'fifoBuffer'    The buffer to work on
*  @return  'letter'        Returns the letter that has been read from the FIFO buffer.
*/
void readFromFIFO(char *letter)
{
    
#ifdef condition /*Conditional Variables*/
    mutex_lock(fifoBuffer->bufferMutex);
    pthread_cleanup_push(cleanup_handler, fifoBuffer->bufferMutex);
    while (is_buffer_empty(fifoBuffer))
    {
        cond_wait(fifoBuffer->buffer_not_empty, fifoBuffer->bufferMutex);                   // cond wait -> buffer empty
    }
    cancelDisable();
    /*CRITICAL SECTION*/
    letter = fifoBuffer->bufferContent[fifoBuffer->readPointer];                        // read letter from buffer
    fifoBuffer->readPointer = bufferPointer_incr(fifoBuffer->readPointer);              // set readpointer
    fifoBuffer->bufferLevel--;
    printf("BufferLevel = %d\n", fifoBuffer->bufferLevel);

    mutex_unlock(fifoBuffer->bufferMutex);
    cancelEnable();
    pthread_cleanup_pop(1);
    cond_signal(fifoBuffer->buffer_not_full);                                          // cond signal -> buffer not empty

#else /*Semaphores*/
    semaphore_wait(fifoBuffer->buffer_elements);                                        // sem buffer elements down
    cancelDisable();
    mutex_lock(fifoBuffer->bufferMutex);
   
    /*CRITICAL SECTION*/
    *letter = fifoBuffer->bufferContent[fifoBuffer->readPointer];                        // read letter from buffer
    fifoBuffer->readPointer = bufferPointer_incr(fifoBuffer->readPointer);              // set readpointer
    fifoBuffer->bufferLevel--;
    printf("BufferLevel = %d\n", fifoBuffer->bufferLevel);

    mutex_unlock(fifoBuffer->bufferMutex);
    cancelEnable();
    semaphore_post(fifoBuffer->buffer_capacity);                                        // sem buffer capacity up

#endif
}



/* ============================================================================
*  @brief   Increases the readPointer or the writePointer of the FIFO buffer.
*           Increases by 1 with each step until buffer size is reached.
*           Then restarts at 1.
*  @param   'fifoBuffer'    The buffer controlled via the pointers.
*  @param   'next'          Either the readPointer or the writePointer to increase.
*  @return  'next'          Returns the correctly moved pointer.
*/
int bufferPointer_incr(int next)
{
    return (next + 1) % BUFFER_SIZE;
}



/* ============================================================================
*  @brief   If the writePointer and the readPointer are at the same level,
*           the FIFO buffer is empty.  
*  @param   'fifoBuffer'    The buffer to be checked.  
*  @return  'true'          if the buffer is empty else 'false'.
*/
int is_buffer_empty(FIFOBuffer *fifoBuffer)
{
    return (fifoBuffer->writePointer == fifoBuffer->readPointer);//in -> write out >read
}



/* ============================================================================
*  @brief   If the writePointer+1 is at the same level as the readPointer,
*           the buffer is full.
*  @param   'fifoBuffer'    The buffer to be checked.  
*  @return  'true'          if the buffer is full else 'false'.
*/
int is_buffer_full(FIFOBuffer *fifoBuffer)
{
    return (bufferPointer_incr(fifoBuffer->writePointer) == fifoBuffer->readPointer);
}





/* ============================================================================
 * @file        : fifo.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the FIFO buffer with it's functionalities
 * ============================================================================
 */


#include "fifo.h"


/* @brief   Initializes a FIFO buffer
*/
FIFOBuffer *make_fifoBuffer()
{
    FIFOBuffer *fifoBuffer = (FIFOBuffer *)check_malloc(sizeof(FIFOBuffer));
    fifoBuffer->bufferContent[BUFFER_SIZE] = (FIFOBuffer *)check_malloc(BUFFER_SIZE * sizeof(char));
    fifoBuffer->readPointer = 0;
    fifoBuffer->writePointer = 0;
    fifoBuffer->fifoMutex = make_mutex();
#ifdef condition
    fifoBuffer->buffer_empty = make_cond();
    fifoBuffer->buffer_not_empty = make_cond();
    fifoBuffer->buffer_full = make_cond();
    fifoBuffer->buffer_not_full = make_cond();
#else
    fifoBuffer->buffer_elements = make_semaphore(0);
    fifoBuffer->buffer_capacity = make_semaphore(BUFFER_SIZE);
#endif // cond
    return fifoBuffer;
}


/* @brief   Write a letter in the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   fifoBuffer  The buffer to work on
*  @param   thread      The thread writing in the buffer
*  @param   letter      The letter to be written in the buffer
*/
void writeInFIFO(FIFOBuffer *fifoBuffer, CPThread *thread, char letter)
{
    mutex_lock(thread->pause);
    
    cancelDisable();
#ifdef condition
    mutex_lock(fifoBuffer->fifoMutex);

    //pthread_cleanup_push(cleanup_handler, thread);

    while (fifoBuffer->bufferLevel >= BUFFER_SIZE)
    {
        cond_wait(fifoBuffer->buffer_full, fifoBuffer->fifoMutex);
    }
   
    /*CRITICAL SECTION*/
    fifoBuffer->bufferContent[fifoBuffer->writePointer] = letter;
	fifoBuffer->bufferLevel ++;
	fifoBuffer->writePointer++;
        printf("BufferLevel: %d", fifoBuffer->bufferLevel);

    //pthread_cleanup_pop(1);

    cond_signal(fifoBuffer->buffer_not_full);

    mutex_unlock(fifoBuffer->fifoMutex);

#else
    cancelDisable();
    semaphore_wait(fifoBuffer->buffer_capacity);    //buffer capacity down
    mutex_lock(fifoBuffer->fifo)Mutex;

    /*CRITICAL SECTION*/
    fifoBuffer->bufferContent[fifoBuffer->writePointer] = letter;
	fifoBuffer->bufferLevel ++;
	fifoBuffer->writePointer++;
        printf("BufferLevel: %d", fifoBuffer->bufferLevel);

    mutex_unlock(fifoBuffer->fifoMutex);
    semaphore_post(fifoBuffer->buffer_elements);    // buffer elements up
    cancelEnable();
#endif // cond
    cancelEnable();
    mutex_unlock(thread->pause);


	if (fifoBuffer->writePointer >= BUFFER_SIZE)
	{
		fifoBuffer->writePointer = 0;
	}
}


/* @brief   Read a letter from the FIFO buffer. 
*           This function is secured for synchronized access.
*  @param   fifoBuffer  The buffer to work on
*  @param   thread      The thread reading from the buffer
*/
char readFromFIFO(FIFOBuffer *fifoBuffer, CPThread *thread)
{
    char letter;

    mutex_lock(thread->pause);
    cancelDisable();

#ifdef condition
    mutex_lock(fifoBuffer->fifoMutex);

    //pthread_cleanup_push(cleanup_handler, thread);

    while (fifoBuffer->bufferLevel == 0)
    {
        cond_wait(fifoBuffer->buffer_empty, fifoBuffer->fifoMutex);         // cond wait -> buffer empty
    }
    /*CRITICAL SECTION*/
    letter = fifoBuffer->bufferContent[fifoBuffer->readPointer];
    fifoBuffer->bufferLevel --;
	fifoBuffer->readPointer ++;
        printf("BufferLevel: %d", fifoBuffer->bufferLevel);

    //pthread_cleanup_pop(1);
    mutex_unlock(fifoBuffer->fifoMutex);
    cond_signal(fifoBuffer->buffer_not_empty);
#else
    cancelDisable();
    semaphore_wait(fifoBuffer->buffer_elements);            //buffer elements down
    mutex_lock(fifoBuffer->fifoMutex);

    /*CRITICAL SECTION*/
    letter = fifoBuffer->bufferContent[fifoBuffer->readPointer];
    fifoBuffer->bufferLevel --;
	fifoBuffer->readPointer ++;
        printf("BufferLevel: %d", fifoBuffer->bufferLevel);

    mutex_unlock(fifoBuffer->fifoMutex);
    semaphore_post(fifoBuffer->buffer_capacity);             // buffer capacity up
    cancelEnable();
#endif
    cancelEnable();
    mutex_unlock(thread->pause);

    
	if (fifoBuffer->readPointer >= BUFFER_SIZE) // if buffer size is reached
	{
		fifoBuffer->readPointer = 0; // start from the beginning again
	}

    return letter;
}
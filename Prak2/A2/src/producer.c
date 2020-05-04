/* ============================================================================
 * Name        : producer.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the produce function for the producer threads.
 * ============================================================================
 */

#include "producer.h"

#include <unistd.h>
/* ============================================================================
*  @brief   The Producer Handler keeps the Producer threads using the 'producer'
*           function to write a letter from the alphabet to the FIFO buffer,
*           print the letter and then pause for 2 seconds.
*           Producer_1 submits a lower case letter and Producer_2 an upper case letter.
*  @param   'thread'    Producer thread
*/
void *producerHandler(CPThread *thread, char arg)
{
    printf("producerHandler entered\n");

    //while (PRODUCER_THREAD_ACTIVE)
    while (1)
    {
        readFromQueue(thread->queue, thread->fifoBuffer->bufferContent);
        (*thread->queue->header->routineForTask)(thread->fifoBuffer);
        sleep(THREE_SECONDS);
    }
    pthread_exit(NULL);
}

/* ============================================================================
*  @brief   A Producer thread uses this function to write a letter into the FIFO buffer.
*  @param   'fifoBuffer'    The buffer to put the letter into.
*  @param   'letter'        The letter to write
*  @param   'thread'        Producer_1 or Producer_2
*/
void producer(FIFOBuffer *fifoBuffer, char letter)
{
    writeInFIFO(fifoBuffer, *(fifoBuffer->bufferContent));
}

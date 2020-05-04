/* ============================================================================
 * Name        : consumer.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the consume function for the consumer thread.
 * ============================================================================
 */
#include <stddef.h>
#include <unistd.h>
#include "consumer.h"
#include "general.h"
#include "queue.h"

/* ============================================================================
*  @brief   The Consumer Handler keeps the Consumer thread using the 'consumer'
*           function to read a letter from the FIFO buffer, print the letter 
*           and then pause for 3 seconds.
*  @param   thread      Consumer thread
*/
void *consumerHandler(CPThread *thread)
{
    printf("consumerHandler entered\n");
    while (CONSUMER_THREAD_ACTIVE)
    {
        char buffer;
        readFromQueue(thread->queue, &buffer);
        (*thread->queue->header->routineForTask)(thread->fifoBuffer);
        sleep(TWO_SECONDS);
    }
}

/* ============================================================================
*  @brief   A Consumer thread uses this function to take a letter from the 
*           FIFO buffer. 
*  @param   fifoBuffer  The FIFO buffer to read the letter from.
*  @param   thread      Consumer thread
*  @return  letter      Returns the letter that has been read from the FIFO buffer
*/
char consumer(FIFOBuffer *fifoBuffer)
{
    printf("consumer active\n");
    char letter;

    letter = readFromFIFO(fifoBuffer);

    printf("Consumer nimmt %c aus Puffer \n", letter);

    return letter;
}

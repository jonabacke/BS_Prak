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
void *consumerHandler(Queue *queue)
{
    printf("consumerHandler entered\n");
    while (CONSUMER_THREAD_ACTIVE)
    {
        char buffer;
        readFromQueue(queue, &buffer);
        (*queue->header->routineForTask)(&buffer);
        sleep(TWO_SECONDS);
    }
    pthread_exit(NULL);
}

/* ============================================================================
*  @brief   A Consumer thread uses this function to take a letter from the 
*           FIFO buffer. 
*  @param   fifoBuffer  The FIFO buffer to read the letter from.
*  @param   thread      Consumer thread
*  @return  letter      Returns the letter that has been read from the FIFO buffer
*/
void consumer(char *letter)
{
    printf("consumer active\n");

    readFromFIFO(letter);

    printf("Consumer nimmt %c aus Puffer \n", *letter);
}

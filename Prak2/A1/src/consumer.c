/* ============================================================================
 * Name        : consumer.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the consume function for the consumer thread.
 * ============================================================================
 */


#include "consumer.h"



/* ============================================================================
*  @brief   The Consumer Handler keeps the Consumer thread using the 'consumer'
*           function to read a letter from the FIFO buffer, print the letter 
*           and then pause for 3 seconds.
*  @param   thread      Consumer thread
*/
void *consumerHandler(CPThread *thread)
{   
    while (CONSUMER_THREAD_ACTIVE)
    {
        printf("%s nimmt %c aus Puffer \n", thread->name, consumer(thread->fifoBuffer, thread));
        sleep(THREE_SECONDS);
    }
    printf("%s wurde terminiert.\n", thread->name);
}



/* ============================================================================
*  @brief   A Consumer thread uses this function to take a letter from the 
*           FIFO buffer. 
*  @param   fifoBuffer  The FIFO buffer to read the letter from.
*  @param   thread      Consumer thread
*  @return  letter      Returns the letter that has been read from the FIFO buffer
*/
char consumer(FIFOBuffer *fifoBuffer, CPThread *thread)
{
    printf("consumer active\n");
    char letter;
    
    mutex_lock(thread->pauseMutex);
    pthread_cleanup_push(cleanup_handler, thread->pauseMutex);
    letter = readFromFIFO(fifoBuffer);
    pthread_cleanup_pop(1);
    mutex_unlock(thread->pauseMutex);

    return letter;
}





/* ============================================================================
 * Name        : producer.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the produce function for the producer threads.
 * ============================================================================
 */


#include "producer.h"



/* ============================================================================
*  @brief   The Producer Handler keeps the Producer threads using the 'producer'
*           function to write a letter from the alphabet to the FIFO buffer,
*           print the letter and then pause for 2 seconds.
*           Producer_1 submits a lower case letter and Producer_2 an upper case letter.
*  @param   'thread'    Producer thread
*/
void *producerHandler(CPThread *thread)
{
    printf("producerhandler entered\n");
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    //while (PRODUCER_THREAD_ACTIVE)
    while(thread->flag)
    {
        char letter;
        
        for (int i = 0; i < sizeof(alphabet); i++)
        {
            if (producerThread_1 == thread)
            {
                letter = tolower(alphabet[i]);
            }
            else
            {
                letter = alphabet[i];
            }
            
            producer(thread->fifoBuffer, letter, thread);
            printf("%s schreibt %c in Puffer \n", thread->name, letter);
            sleep(TWO_SECONDS);
        }
    }
    printf("%s wurde terminiert.\n", thread->name);
}


/* ============================================================================
*  @brief   A Producer thread uses this function to write a letter into the FIFO buffer.
*  @param   'fifoBuffer'    The buffer to put the letter into.
*  @param   'letter'        The letter to write
*  @param   'thread'        Producer_1 or Producer_2
*/
void producer(FIFOBuffer *fifoBuffer, char letter, CPThread *thread)
{
    mutex_lock(thread->pauseMutex);
    pthread_cleanup_push(cleanup_handler, thread->pauseMutex);
        writeInFIFO(fifoBuffer, letter);
    pthread_cleanup_pop(1);
    //mutex_unlock(thread->pauseMutex);
}



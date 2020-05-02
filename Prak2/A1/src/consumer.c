/* ============================================================================
 * Name        : consumer.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the consume function for the consumer thread.
 * ============================================================================
 */


#include "consumer.h"


/* @brief   A Consumer thread uses this function to take a letter from the 
*           FIFO buffer and print it. After reading the letter the thread takes 
*           a break for 3 seconds.
*  @param   fifoBuffer  The buffer to read the letter from.
*  @param   thread      Consumer
*/
void *consume(FIFOBuffer *fifoBuffer, CPThread *thread)
{
    printf("%s wurde aktiviert.", thread->name);
    while(1)
    {
        char letter = readFromFIFO(fifoBuffer, thread); 
        printf("%s nimmt %c aus dem FIFO-Puffer.\n", thread->name, letter);   
        sleep(FOR_3_SECONDS);
    }
    printf("%s wurde terminiert.", thread->name);
}




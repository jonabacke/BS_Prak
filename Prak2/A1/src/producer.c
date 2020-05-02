/* ============================================================================
 * Name        : producer.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides the produce function for the producer threads.
 * ============================================================================
 */


#include "producer.h"


/* @brief   A Producer thread uses this function to put a letter from the alphabet 
*           into the FIFO buffer. Producer_1 submits a lower case letter and 
*           Producer_2 an upper case letter. After submitting the letter the thread 
*           takes a break for 2 seconds.
*  @param   fifoBuffer  The buffer to put the letter into.
*  @param   thread      Producer_1 or Producer_2
*/
void *produce(FIFOBuffer *fifoBuffer, CPThread *thread)
{
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    printf("%s wurde aktiviert.", thread->name);
    while(1)
    {
        char letter;

        for (int i = 0; i < sizeof(alphabet); i ++)
        {
            if ("producer_1" == thread->name)
                {
            letter = tolower(alphabet[i]);
            }
            else
            {
                letter = alphabet[i];
            }      
            writeInFIFO(fifoBuffer, thread, letter);
            printf("%s schreibt %c in den FIFO-Puffer.\n", thread->name, letter);
            sleep(FOR_2_SECONDS);
        }
    }   
    printf("%s wurde terminiert.", thread->name);
}




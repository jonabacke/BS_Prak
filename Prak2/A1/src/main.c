/* ============================================================================
 * @file        : main.c
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This is the main program file
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "general.h"
#include "controler.h"
#include "fifo.h"
#include "thread.h"


void cond_destroy(Cond *cond) {
    int n = pthread_cond_destroy(cond);
    HANDLE_ERR(n);
}


/* @brief   In the main the threads are created and at the end their remains destroyed.
*/
int main(int argc, char const *argv[])
{
    FIFOBuffer *fifoBuffer = make_FIFOBuffer();
    producerThread_1 = makeConsumerProducerThread(producerHandler, fifoBuffer, "Producer_1");
    producerThread_2 = makeConsumerProducerThread(producerHandler, fifoBuffer, "Producer_2");
    consumerThread = makeConsumerProducerThread(consumerHandler, fifoBuffer, "Consumer");
    pthread_create(&controlThread, NULL, control, NULL);
    
    pthread_join(producerThread_1->thread, NULL);
    pthread_join(producerThread_2->thread, NULL);
    pthread_join(consumerThread->thread, NULL);

    


    // ENDING THE PROGRAM

            printf("Thread canceling.....\n");
            /*tc1-tc3 are cancelled in the control module*/
            int tc4 = pthread_cancel(controlThread);
            HANDLE_ERR(tc4);
            printf("Destroying Mutex.....\n");
            pthread_mutex_destroy(fifoBuffer->bufferMutex);
    
        #ifdef condition /*Conditional Variables*/
            // int cd1 = cond_destroy(fifoBuffer->buffer_empty);
            // HANDLE_ERR(cd1);
            cond_destroy(fifoBuffer->buffer_not_empty);
            // int cd3 = cond_destroy(fifoBuffer->buffer_full);
            // HANDLE_ERR(cd3);
            cond_destroy(fifoBuffer->buffer_not_full);
        #else /*Semaphores*/
            printf("Destroying Semaphores.....\n");
            int sd1 = sem_destroy(fifoBuffer->buffer_elements);
            HANDLE_ERR(sd1);
            int sd2 = sem_destroy(fifoBuffer->buffer_capacity);
            HANDLE_ERR(sd2);
        #endif

            printf("Freeing memory.....\n");
		#ifdef condition /*Conditional Variables*/
            // free(fifoBuffer->buffer_empty);
            free(fifoBuffer->buffer_not_empty);
            // free(fifoBuffer->buffer_full);
            free(fifoBuffer->buffer_not_full);
		#else /*Semaphores*/
            free(fifoBuffer->buffer_elements);
            free(fifoBuffer->buffer_capacity);
		#endif

            free(fifoBuffer->bufferMutex);
            free(fifoBuffer->bufferContent);
            free(fifoBuffer);


            printf("................ end of main process.\n");
    pthread_join(controlThread, NULL);
    return EXIT_SUCCESS;
}

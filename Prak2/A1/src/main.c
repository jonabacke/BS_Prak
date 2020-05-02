/* ============================================================================
 * @file        : main.c
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This is the main program file
 * ============================================================================
 */


#include "general.h"
#include "producer.h"
#include "consumer.h"
#include "controler.h"

CPThread *Producer_1 = NULL;
CPThread *Producer_2 = NULL;
CPThread *Consumer = NULL;

pthread_t threadControl;


/* @brief   In the main the threads are created and at the end their remains destroyed.
*/
int main(int argc, char const *argv[])
{
    FIFOBuffer *fifoBuffer = make_fifoBuffer();
    Producer_1 = makeConsumerProducerThread(fifoBuffer, "Producer_1");
    Producer_2 = makeConsumerProducerThread(fifoBuffer, "Producer_2");
    Consumer = makeConsumerProducerThread(fifoBuffer, "Consumer");

    int tcr1 = pthread_create(Producer_1->thread, NULL, produce, fifoBuffer);
    HANDLE_ERR(tcr1);
    int tcr2 = pthread_create(Producer_2->thread, NULL, produce, fifoBuffer);
    HANDLE_ERR(tcr2);
    int tcr3 = pthread_create(Consumer->thread, NULL, consume, fifoBuffer);
    HANDLE_ERR(tcr3);
    int tcr4 = pthread_create(&threadControl, NULL, control, NULL);
    HANDLE_ERR(tcr4);

    int tj1 = pthread_join(Producer_1->thread, NULL);
    HANDLE_ERR(tj1);
    int tj2 = pthread_join(Producer_2->thread, NULL);
    HANDLE_ERR(tj2);
    int tj3 = pthread_join(Consumer->thread, NULL);
    HANDLE_ERR(tj3);

    
    // ENDING THE PROGRAM

            printf("Thread canceling.....\n");
            /*tc1-tc3 are cancelled in the control module*/
            int tc4 = pthread_cancel(threadControl);
            HANDLE_ERR(tc4);

            printf("Destroying Mutex.....\n");
            int md1 = pthread_mutex_destroy(fifoBuffer->fifoMutex);
            HANDLE_ERR(md1);
    
        #ifdef condition
           /*int cd1 = cond_destroy(fifoBuffer->nonEmpty);
            HANDLE_ERR(cd1);
            int cd2 = cond_destroy(fifoBuffer->nonFull);
            HANDLE_ERR(cd2);*/
        #else
            printf("Destroying Semaphores.....\n");
            int sd1 = sem_destroy(fifoBuffer->buffer_elements);
            HANDLE_ERR(sd1);
            int sd2 = sem_destroy(fifoBuffer->buffer_capacity);
            HANDLE_ERR(sd2);
        #endif

            printf("Freeing memory.....\n");
		#ifdef condition
            free(fifoBuffer->buffer_empty);
            free(fifoBuffer->buffer_not_empty);
            free(fifoBuffer->buffer_full);
            free(fifoBuffer->buffer_not_full);
		#else
            free(fifoBuffer->buffer_elements);
            free(fifoBuffer->buffer_capacity);
		#endif

            free(fifoBuffer->fifoMutex);
            //free(fifoBuffer->bufferContent[BUFFER_SIZE]);
            free(fifoBuffer);
            free(Producer_1);
            free(Producer_2);
            free(Consumer);


            printf("................ end of main process.\n");

    return EXIT_SUCCESS;
}

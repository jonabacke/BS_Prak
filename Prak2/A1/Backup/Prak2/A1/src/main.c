#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "general.h"
#include "controler.h"
#include "fifo.h"
#include "thread.h"
#include "myError.h"




pthread_t threadControl;



int main(int argc, char const *argv[])
{
    FIFOStack *stack = make_stack(10);
    producerThreadOne = makeConsumerProducerThread(producerHandler, stack, "P!", "abcdefghijklmnopqrstuvwxyz");
    producerThreadTwo = makeConsumerProducerThread(producerHandler, stack, "P2", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    consumerThreadOne = makeConsumerProducerThread(consumerHandler, stack, "C1", NULL);
    pthread_create(&threadControl, NULL, control, NULL);

    pthread_join(producerThreadOne->thread, NULL);
    pthread_join(producerThreadTwo->thread, NULL);
    pthread_join(consumerThreadOne->thread, NULL);

    pthread_mutex_destroy(stack->fifo);


    // ENDING THE PROGRAM

            printf("Thread canceling.....\n");
            /*tc1-tc3 are cancelled in the control module*/
            int tc4 = pthread_cancel(threadControl);
            HANDLE_ERR(tc4);

            printf("Destroying Mutex.....\n");
            int md1 = pthread_mutex_destroy(stack->fifo);
            HANDLE_ERR(md1);
    
        #ifdef condition
           /*int cd1 = cond_destroy(stack->nonEmpty);
            HANDLE_ERR(cd1);
            int cd2 = cond_destroy(stack->nonFull);
            HANDLE_ERR(cd2);*/
        #else
            printf("Destroying Semaphores.....\n");
            int sd1 = sem_destroy(stack->items);
            HANDLE_ERR(sd1);
            int sd2 = sem_destroy(stack->spaces);
            HANDLE_ERR(sd2);
        #endif

            printf("Freeing memory.....\n");
		#ifdef condition
            free(stack->nonEmpty);
            free(stack->nonFull);
		#else
            free(stack->items);
            free(stack->spaces);
		#endif

            free(stack->fifo);
            free(stack->array);
            free(stack);


            printf("................ end of main process.\n");

    return EXIT_SUCCESS;
}
/* ============================================================================
 * @file        : controler.c
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the control thread which can control and 
 *                terminate the threads.
 * ============================================================================
 */


#include "controler.h"

#define AMOUNTPRODUCER 	5
#define AMOUNTCONSUMER	5

Queue *producerQueue;
Queue *consumerQueue;

pthread_t producerGenerator;
pthread_t consumerGenerator;

pthread_t *producerThread;
pthread_t *consumerThread;

/* ============================================================================
*  @brief   Function used by the control thread. Reads key input, can pause 
*           and terminate the created threads.
*/
void *control(void *not_used)
{
	char *producerQueueName = "/producer";
	char *consumerQueueName = "/consumer";

	producerQueue = initProducerQueue(producerQueueName);
	consumerQueue = initConsumerQueue(consumerQueueName);

	make_thread(&producerGenerator, runProducerQueue, producerQueue);
	make_thread(&consumerGenerator, runConsumerQueue, consumerQueue);

	producerThread = check_malloc(sizeof(pthread_t) * AMOUNTPRODUCER);
	consumerThread = check_malloc(sizeof(pthread_t) * AMOUNTCONSUMER);

	for (int i = 0; i < AMOUNTPRODUCER; ++ i) {
		make_thread(&producerThread[i], producerHandler, producerQueue);
	}

	for (int i = 0; i < AMOUNTCONSUMER; ++ i) {
		make_thread(&consumerThread[i], consumerHandler, consumerQueue);
	}


    while (1)
    {   
        /*READING AND EVALUATING KEY INPUT*/
        char order = getchar();

        switch (tolower(order))
        {
        case 'p':
            toggleThread(producerQueue);
            printf("--- toggle Producer\n");
            break;

        case 'c':
            toggleThread(consumerQueue);
            printf("--- toggle consumer\n");
            break;

        case 'q':
            printf("--- Programm wird beendet.\n");
            cancelAll(producerQueueName, consumerQueueName);
            break;

        case 'h':
            printCommands();
            break;

        default:
            break;
        }
    }
}



/* ============================================================================
*  @brief   Toggles (pauses) the producer or consumer threads.
*  @param   Thread to be toggled.
*/
void toggleThread(Queue *queue)
{
    if (queue->flag)
    {
    	queue->flag = TURN_OFF;
        mutex_lock(queue->block);
    }
    else
    {
    	queue->flag = TURN_ON;
        mutex_unlock(queue->block);
    }
}



/* ============================================================================
*  @brief   Prints all available commands when 'h' key is pressed.
*/
void printCommands()
{
    printf( "\n\n -----------------------------------\n" 
            "Commands:\n"
            "1\t start/stop 'Producer_1'\n"
            "2\t start/stop 'Producer_2'\n" 
            "c/C\t start/stop 'Consumer'\n"
            "q/Q\t terminate the system\n" 
            "h\t print commands\n"
            "------------------------------------\n\n\n ");
}



/* ============================================================================
*  @brief   Terminates the producer and consumer threads.
*/
void cancelAll(char *producerQueueName, char *consumerQueueName)
{
	printf("cancelAll wird betreten \n");
    int tc1 = pthread_cancel(producerGenerator);
    HANDLE_ERR(tc1);
    int tc2 = pthread_cancel(consumerGenerator);
    HANDLE_ERR(tc2);

	for (int i = 0; i < AMOUNTPRODUCER; ++ i) {
		int tc3 = pthread_cancel(producerThread[i]);
	    HANDLE_ERR(tc3);
	}

	for (int i = 0; i < AMOUNTCONSUMER; ++ i) {
		int tc4 = pthread_cancel(consumerThread[i]);
	    HANDLE_ERR(tc4);
	}
    if (producerQueue->flag == TURN_OFF)
    {
        toggleThread(producerQueue);
    }
    if (consumerQueue->flag == TURN_OFF)
    {
        toggleThread(consumerQueue);
    }

    deleteAll(producerQueueName, consumerQueueName);
    pthread_exit(NULL);
}


void deleteAll(char *producerQueueName, char *consumerQueueName)
{
	// delete all mellocs,mutex sem...

	destroyTaskQueue(producerQueueName);

	closeTaskQueue(producerQueue->queue);

	destroyTaskQueue(consumerQueueName);

	closeTaskQueue(consumerQueue->queue);



//     ENDING THE PROGRAM

     printf("Destroying Mutex.....\n");



     int f = pthread_mutex_destroy(producerQueue->bufferMutex);
     HANDLE_ERR(f);
     f = pthread_mutex_destroy(producerQueue->block);
     HANDLE_ERR(f);
     f = pthread_mutex_destroy(consumerQueue->bufferMutex);
     HANDLE_ERR(f);
     f = pthread_mutex_destroy(consumerQueue->block);
     HANDLE_ERR(f);

#ifdef condition /*Conditional Variables*/
     f = cond_destroy(producerQueue->buffer_not_empty);
     HANDLE_ERR(f);
     int f = cond_destroy(producerQueue->buffer_not_full);
     HANDLE_ERR(f);
#else /*Semaphores*/
    printf("Destroying Semaphores.....\n");
     f = sem_destroy(producerQueue->buffer_elements);
    HANDLE_ERR(f);
     f = sem_destroy(producerQueue->buffer_capacity);
    HANDLE_ERR(f);
#endif

    printf("Freeing memory.....\n");
#ifdef condition /*Conditional Variables*/
    free(producerQueue->buffer_not_empty);
    free(producerQueue->buffer_not_full);
#else /*Semaphores*/
    free(producerQueue->buffer_elements);
    free(producerQueue->buffer_capacity);
#endif



#ifdef condition /*Conditional Variables*/
     f = cond_destroy(consumerQueue->buffer_not_empty);
     HANDLE_ERR(f);
     f = cond_destroy(consumerQueue->buffer_not_full);
     HANDLE_ERR(f);
#else /*Semaphores*/
    printf("Destroying Semaphores.....\n");
     f = sem_destroy(consumerQueue->buffer_elements);
    HANDLE_ERR(f);
     f = sem_destroy(consumerQueue->buffer_capacity);
    HANDLE_ERR(f);
#endif

    printf("Freeing memory.....\n");
#ifdef condition /*Conditional Variables*/
    free(consumerQueue->buffer_not_empty);
    free(consumerQueue->buffer_not_full);
#else /*Semaphores*/
    free(consumerQueue->buffer_elements);
    free(consumerQueue->buffer_capacity);
#endif

    free(producerQueue->bufferMutex);
    free(producerQueue->block);
    free(producerQueue);

    free(consumerQueue->bufferMutex);
    free(consumerQueue->block);
    free(consumerQueue);

     printf("................ end of destroy process.\n");
}





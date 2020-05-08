/* ============================================================================
 * @file        : controler.c
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the control thread which can control and 
 *                terminate the threads.
 * ============================================================================
 */


#include "controler.h"
#include "general.h"

#define AMOUNTPRODUCER 	5
#define AMOUNTCONSUMER	5

Queue *producerQueue;
Queue *consumerQueue;

pthread_t producerGenerator;
pthread_t consumerGenerator;

pthread_t producerThread[AMOUNTPRODUCER];
pthread_t consumerThread[AMOUNTCONSUMER];

/* ============================================================================
*  @brief   Function used by the control thread. Reads key input, can pause 
*           and terminate the created threads.
*/
void *control(void *not_used)
{
	producerQueue = initProducerQueue();
	consumerQueue = initConsumerQueue();

	make_thread(&producerGenerator, runProducerQueue, producerQueue);
	make_thread(&consumerGenerator, runConsumerQueue, consumerQueue);

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
            printf("--- toggle Producer_1\n");
            break;

        case 'c':
            toggleThread(consumerQueue);
            printf("--- toggle consumer\n");
            break;

        case 'q':
            printf("--- Programm wird beendet.\n");
            cancelAll();
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
void cancelAll() 
{
	printf("cancelAll wird betreten \n");
    if (producerQueue->flag == TURN_OFF)
    {
        toggleThread(producerQueue);
    }
    if (consumerQueue->flag == TURN_OFF)
    {
        toggleThread(consumerQueue);
    }
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

    deleteAll();
    pthread_exit(NULL);
}


void deleteAll()
{
	// delete all mellocs,mutex sem...
}





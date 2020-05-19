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

#include "controler.h"
#include "consumerGeneraterThread.h"
#include "producerGeneraterThread.h"

pthread_t threadControl;
//void cond_destroy(Cond *cond)
//{
//    int n = pthread_cond_destroy(cond);
//    HANDLE_ERR(n);
//}


/* @brief   In the main the threads are created and at the end their remains destroyed.
*/
int main(int argc, char const *argv[])
{
	FIFOBuffer *fifoBuffer =  make_FIFOBuffer();
    make_thread(&threadControl, control, NULL);


    pthread_join(threadControl, NULL);

//	Queue *queue = initProducerQueue();

//    mqd_t producerQueue = createTaskQueue("/producer", 10, sizeof(char));

//	struct TaskHeader header;
//	header.argSize = sizeof(char);
//	header.routineForTask = writeInFIFO;
//	char peter = 'a';
//
//	sendToTaskQueue(producerQueue, header, &peter, 1);
//
//	printf("rein geschrieben \n");
//
//
//	struct TaskHeader header2;
//	char hans;
//	receiveFromTaskQueue(producerQueue, &header2, &hans, sizeof(char));
//
//	printf("raus gelesen\n");
//
//	destroyTaskQueue("/producer");
//
//	closeTaskQueue(producerQueue);
//
//





    // ENDING THE PROGRAM

     /*tc1-tc3 are cancelled in the control module*/
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

    return EXIT_SUCCESS;
}

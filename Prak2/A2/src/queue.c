
#include "queue.h"





//#define QUEUE_SIZE 		10





void readFromQueue(Queue *queue, char *argBuf, struct TaskHeader *header)
{
#ifdef condition /*Conditional Variables*/
    mutex_lock(queue->bufferMutex);
    pthread_cleanup_push(cleanup_handler, queue->bufferMutex);
    while (is_queue_empty(queue))
    {
        cond_wait(queue->buffer_not_empty, queue->bufferMutex);                   // cond wait -> buffer empty
    }
    cancelDisable();
    /*CRITICAL SECTION*/
    receiveFromTaskQueue(queue->queue, header, argBuf, sizeof(char));
    queue->readPointer = queuePointer_incr(queue->readPointer);

    mutex_unlock(queue->bufferMutex);
    cancelEnable();
    pthread_cleanup_pop(1);
    cond_signal(queue->buffer_not_full);                                          // cond signal -> buffer not empty

#else /*Semaphores*/
    semaphore_wait(queue->buffer_elements);                                        // sem buffer elements down
    cancelDisable();
    mutex_lock(queue->bufferMutex);

    /*CRITICAL SECTION*/
    receiveFromTaskQueue(queue->queue, header, argBuf, sizeof(char));
    queue->readPointer = queuePointer_incr(queue->readPointer);

    mutex_unlock(queue->bufferMutex);
    cancelEnable();
    semaphore_post(queue->buffer_capacity);                                        // sem buffer capacity up

#endif
}





void writeIntoQueue(Queue *queue, const char *arg, struct TaskHeader header)
{
#ifdef condition /*Conditional variables*/
    mutex_lock(queue->bufferMutex);
    pthread_cleanup_push(cleanup_handler, queue->bufferMutex);
    while (is_queue_full(queue))
    {
        cond_wait(queue->buffer_not_full, queue->bufferMutex);                		//cond wait -> buffer full
    }
	cancelDisable();

    /*CRITICAL SECTION*/
    sendToTaskQueue(queue->queue, header, arg, 0);
    queue->writePointer = queuePointer_incr(queue->writePointer);

    mutex_unlock(queue->bufferMutex);
    pthread_cleanup_pop(1);
    cond_signal(queue->buffer_not_empty);  												//cond signal -> buffer not full
    cancelEnable();

#else /*Semaphores*/
    semaphore_wait(queue->buffer_capacity);                                            // sem buffer capacity down
    cancelDisable();
    mutex_lock(queue->bufferMutex);

    /*CRITICAL SECTION*/
    sendToTaskQueue(queue->queue, header, arg, 0);
    queue->writePointer = queuePointer_incr(queue->writePointer);

    mutex_unlock(queue->bufferMutex);
    cancelEnable();
    semaphore_post(queue->buffer_elements);                                            // sem buffer elements up

#endif // cond
}




int is_queue_empty(Queue *queue) {
    return (queue->writePointer == queue->readPointer);
}


int queuePointer_incr(int next)
{
    return (next + 1) % QUEUE_SIZE;
}

int is_queue_full(Queue *queue)
{
    return (queuePointer_incr(queue->writePointer) == queue->readPointer);
}

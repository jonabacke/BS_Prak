
#include "consumerGeneraterThread.h"
#include <unistd.h>
#include "consumer.h"




pthread_t initConsumerQueue(void *buffer)
{
    mqd_t consumerQueue = createTaskQueue("/consumer", sizeConsumerQueue, sizeof(char));

    Queue *queue = check_malloc(sizeof(Queue));
    queue->length = sizeConsumerQueue;
    queue->queue = consumerQueue;
    queue->bufferMutex = make_mutex();
    queue->header = check_malloc(sizeof(struct TaskHeader));
    queue->header->routineForTask = readFromFIFO;
    queue->header->argSize = sizeof(FIFOBuffer);
    queue->readPointer = 0;
    queue->writePointer = 0;
    queue->block = make_mutex();
#ifdef condition /*CONDITION VARIABLES*/
    queue->buffer_not_empty = make_cond();
    queue->buffer_not_full = make_cond();
#else /*SEMAPHORES*/
    queue->buffer_elements = make_semaphore(0);
    queue->buffer_capacity = make_semaphore(sizeConsumerQueue - 1);
#endif

    for (int i = 0; i < 5; i++)
    {
        makeConsumerProducerThread(consumerHandler, buffer, queue);
    }
    CPThread *consumerThread = makeConsumerProducerThread(runConsumerQueue, buffer, queue);
    return consumerThread->thread;

}

void *runConsumerQueue(Queue *queue)
{

    while (1)
    {
        char result;
        printf("hi7 \n");
        mutex_lock(queue->block);
        writeIntoQueue(queue, &result);
        mutex_unlock(queue->block);
        sleep(TWO_SECONDS);
    }

    pthread_exit(NULL);
}

#include "consumerGeneraterThread.h"
#include <unistd.h>
#include "consumer.h"

Queue *initConsumerQueue(char *consumerQueueName)
{
    printf("generateConsumerQueue\n");
    mqd_t consumerQueue = createTaskQueue(consumerQueueName, sizeConsumerQueue, sizeof(char));

    Queue *queue = check_malloc(sizeof(Queue));
    queue->length = sizeConsumerQueue;
    queue->queue = consumerQueue;
    queue->bufferMutex = make_mutex();
    queue->block = make_mutex();
    queue->header = check_malloc(sizeof(struct TaskHeader));
    queue->header->routineForTask = &readFromFIFO;
    queue->header->argSize = sizeof(char);
    queue->readPointer = 0;
    queue->writePointer = 0;
    queue->flag = TURN_ON;
#ifdef condition /*CONDITION VARIABLES*/
    queue->buffer_not_empty = make_cond();
    queue->buffer_not_full = make_cond();
#else /*SEMAPHORES*/
    queue->buffer_elements = make_semaphore(0);
    queue->buffer_capacity = make_semaphore(sizeConsumerQueue - 1);
#endif
    return queue;
}

void *runConsumerQueue(Queue *queue)
{
	struct TaskHeader header;
	header.argSize = sizeof(char);
	header.routineForTask = readFromFIFO;

    while (1)
    {
        char result;
        mutex_lock(queue->block);
        mutex_unlock(queue->block);
        printf("schreibeConsumerTask in Queue\n");
        writeIntoQueue(queue, &result, header);
        sleep(TWO_SECONDS);
    }

    pthread_exit(NULL);
}

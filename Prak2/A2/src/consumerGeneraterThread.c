
#include "consumerGeneraterThread.h"

#define queueSize 10;

void *initConsumerQueue(void *stack)
{
    mqd_t consumerQueue = createTaskQueue("/consumer", sizeConsumerQueue, sizeof(char));
    generateConsumerMutex = make_mutex();
    struct TaskHeader header;
    header.argSize = sizeof(char);
    header.routineForTask = &consumerTask;

    QueueStruct *queue = check_malloc(sizeof(QueueStruct));
    queue->length = sizeConsumerQueue;
    queue->next_in = 0;
    queue->next_out = 0;
    queue->nonEmpty = make_cond();
    queue->nonFull = make_cond();
    queue->schlange = consumerQueue;
    queue->queue = make_mutex();

    char result;
    for (int i = 0; i < 5; i++)
    {
        makeConsumerProducerThread(consumer, stack, sizeConsumerQueue, queue);
    }
    while (1)
    {

        printf("hi7 \n");
        mutex_lock(generateConsumerMutex);
        //sendToTaskQueue(consumerQueue, header, &result, false);
        mutex_unlock(generateConsumerMutex);
        printf("%c aus dem Stack geholt\n", result);
        sleep(1);
    }
}
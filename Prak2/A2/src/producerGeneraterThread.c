
#include "producerGeneraterThread.h"

void *initProducerQueue(void *stack)
{
    mqd_t producerQueue = createTaskQueue("/producer", sizeProducerQueue, sizeof(char));
    generateProducerMutex = make_mutex();
    struct TaskHeader header;
    header.argSize = sizeof(char);
    header.routineForTask = &producerTask;

    QueueStruct *queue = check_malloc(sizeof(QueueStruct));
    queue->length = sizeProducerQueue;
    queue->next_in = 0;
    queue->next_out = 0;
    queue->nonEmpty = make_cond();
    queue->nonFull = make_cond();
    queue->schlange = producerQueue;
    queue->queue = make_mutex();

    for (int i = 0; i < 1; i++)
    {
        makeConsumerProducerThread(producer, stack, sizeProducerQueue, queue);
    }
    while (1)
    {
        char arg = (char)getcharTimeout(1);
        printf("hi8 %c  \n", arg);
        mutex_lock(generateProducerMutex);

        mutex_lock(queue->queue);
        while (queue_full(queue))
        {
            cond_wait(queue->nonEmpty, queue->queue);
        }
        //sendToTaskQueue(producerQueue, header, &arg, false);
        queue->next_in = stack_incr(queue->length, queue->next_in);
        mutex_unlock(queue->queue);
        cond_signal(queue->nonEmpty);
        mutex_unlock(generateProducerMutex);
        printf("%c soll in den Stack gelegt werden\n", arg);
    }
}

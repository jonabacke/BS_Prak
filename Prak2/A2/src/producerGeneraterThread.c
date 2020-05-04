
#include "producerGeneraterThread.h"
#include <unistd.h>

#define sizeProducerQueue 10
#define POOLSIZE 5

pthread_t initProducerQueue(void *buffer)
{
    mqd_t producerQueue = createTaskQueue("/producer", sizeProducerQueue, sizeof(char));

    struct TaskHeader header;
    header.argSize = sizeof(char);
    header.routineForTask = &writeInFIFO;

    Queue *queue = check_malloc(sizeof(Queue));
    queue->length = sizeProducerQueue;
    queue->queue = producerQueue;
    queue->bufferMutex = make_mutex();
    queue->block = make_mutex();
    queue->header = check_malloc(sizeof(struct TaskHeader));
    queue->readPointer = 0;
    queue->writePointer = 0;
    queue->bufferContent = check_malloc(sizeof(char));
#ifdef condition /*CONDITION VARIABLES*/

    queue->buffer_not_empty = make_cond();
    queue->buffer_not_full = make_cond();
#else /*SEMAPHORES*/
    queue->buffer_elements = make_semaphore(0);
    queue->buffer_capacity = make_semaphore(sizeProducerQueue - 1);

#endif

    CPThread *threads[POOLSIZE];

    for (int i = 0; i < POOLSIZE; i++)
    {
        threads[i] = makeConsumerProducerThread(producerHandler, buffer, queue);
    }

    CPThread *producerThread = makeConsumerProducerThread(runProducerQueue, buffer, queue);
    return producerThread->thread;
}

void *runProducerQueue(Queue *queue)
{

    while (1)
    {
        char arg = (char)getcharTimeout(1);
        printf("hi8 %c  \n", arg);
        mutex_lock(queue->block);
        //sendToTaskQueue(producerQueue, header, &arg, false);
        mutex_unlock(queue->block);
        printf("%c soll in den Stack gelegt werden\n", arg);
    }
    pthread_exit(NULL);
}
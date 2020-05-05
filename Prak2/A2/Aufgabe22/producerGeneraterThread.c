
#include "producerGeneraterThread.h"
#include <unistd.h>

#define sizeProducerQueue 10
#define POOLSIZE 5

pthread_t initProducerQueue(void)
{
    printf("generateProducerQueue\n");
    mqd_t producerQueue = createTaskQueue("/producer", sizeProducerQueue, sizeof(char));

    Queue *queue = check_malloc(sizeof(Queue));
    queue->length = sizeProducerQueue;
    queue->queue = producerQueue;
    queue->bufferMutex = make_mutex();
    queue->block = make_mutex();
    queue->header = check_malloc(sizeof(struct TaskHeader));
    queue->header->routineForTask = writeInFIFO;
    queue->header->argSize = sizeof(char);
    queue->readPointer = 0;
    queue->writePointer = 0;
    queue->flag = 1;
#ifdef condition /*CONDITION VARIABLES*/

    queue->buffer_not_empty = make_cond();
    queue->buffer_not_full = make_cond();
#else /*SEMAPHORES*/
    queue->buffer_elements = make_semaphore(0);
    queue->buffer_capacity = make_semaphore(sizeProducerQueue - 1);

#endif

    pthread_t *threads[POOLSIZE];

    for (int i = 0; i < POOLSIZE; i++)
    {
        threads[i] = makeConsumerProducerThread(producerHandler, queue);
    }

    pthread_t producerThread;
    make_thread(producerThread, runProducerQueue, queue);
    return producerThread;
}

void *runProducerQueue(Queue *queue)
{

    while (1)
    {
        char arg = (char)getcharTimeout(1);
        printf("hi8 %c  \n", arg);
        mutex_lock(queue->block);
        pthread_cleanup_push(cleanup_handler, queue->block);
        writeIntoQueue(queue, &arg);
        pthread_cleanup_pop(1);
        mutex_unlock(queue->block);
        printf("%c soll in den Stack gelegt werden\n", arg);
    }
    pthread_exit(NULL);
}

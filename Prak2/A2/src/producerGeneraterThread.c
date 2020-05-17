
#include "producerGeneraterThread.h"
#include <unistd.h>

#define sizeProducerQueue 10
#define POOLSIZE 5
#define ALPHALENGTH 25

Queue *initProducerQueue(char *name)
{
    printf("generateProducerQueue\n");
    mqd_t producerQueue = createTaskQueue(name, sizeProducerQueue, sizeof(char));

    Queue *queue = check_malloc(sizeof(Queue));
    queue->length = sizeProducerQueue;
    queue->queue = producerQueue;
    queue->bufferMutex = make_mutex();
    queue->block = make_mutex();
    queue->header = check_malloc(sizeof(struct TaskHeader));
    queue->header->routineForTask = &writeInFIFO;
    queue->header->argSize = sizeof(char);
    queue->readPointer = 0;
    queue->writePointer = 0;
    queue->flag = TURN_ON;
#ifdef condition /*CONDITION VARIABLES*/
    queue->buffer_not_empty = make_cond();
    queue->buffer_not_full = make_cond();
#else /*SEMAPHORES*/
    queue->buffer_elements = make_semaphore(0);
    queue->buffer_capacity = make_semaphore(sizeProducerQueue - 1);

#endif
    return queue;
}

void *runProducerQueue(Queue *queue)
{
	struct TaskHeader header;
	header.argSize = sizeof(char);
	header.routineForTask = writeInFIFO;

	int pointer = 0;
	char *alpha = "qwertzuioplkjhgfdayxcvbnm";
	char arg;

    while (1)
    {
    	arg = alpha[pointer];
    	pointer ++;
    	pointer = pointer % ALPHALENGTH;


        mutex_lock(queue->block);
        mutex_unlock(queue->block);
        printf("schreibe producerTask in Queue\n");
        writeIntoQueue(queue, &arg, header);
        printf("%c soll in den FIFO gelegt werden\n", arg);
        sleep(ONE_SECOND);
    }
    pthread_exit(NULL);
}

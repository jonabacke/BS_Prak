#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "general.h"
#include "controler.h"
#include "fifo.h"
#include "thread.h"
#include "consumerGeneraterThread.h"
#include "producerGeneraterThread.h"

#define stackSize 10
#define consumerQueueSize 10
#define producerQueueSize 10







pthread_t threadControl;
pthread_t producerQueue;
pthread_t consumerQueue;



int main(int argc, char const *argv[])
{
    FIFOStack *stack = make_stack(stackSize, consumerQueueSize, producerQueueSize);
    for (int i = 0; i < 5; i++)
    {
        makeConsumerProducerThread(producer, stack);
        makeConsumerProducerThread(consumer, stack);
    }
    pthread_create(&threadControl, NULL, control, NULL);
    pthread_create(&producerQueue, NULL, initConsumerQueue, NULL);
    pthread_create(&consumerQueue, NULL, initProducerQueue, NULL);

    pthread_join(threadControl, NULL);
    pthread_join(producerQueue, NULL);
    pthread_join(consumerQueue, NULL);

    return EXIT_SUCCESS;
}

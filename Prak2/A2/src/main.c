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










int main(int argc, char const *argv[])
{
    FIFOStack *stack = make_stack(stackSize);
    pthread_create(&threadControl, NULL, control, NULL);
    pthread_create(&consumerQueue, NULL, initConsumerQueue, stack);
    pthread_create(&producerQueue, NULL, initProducerQueue, stack);
    sleep(1);

    pthread_join(threadControl, NULL);
    pthread_join(producerQueue, NULL);
    pthread_join(consumerQueue, NULL);

    return EXIT_SUCCESS;
}

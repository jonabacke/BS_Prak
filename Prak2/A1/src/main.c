#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "general.h"
#include "controler.h"
#include "fifo.h"
#include "thread.h"






pthread_t threadControl;



int main(int argc, char const *argv[])
{
    FIFOStack *stack = make_stack(10);
    producerThreadOne = makeConsumerProducerThread(producerHandler, stack, "P!", "abcdefghijklmnopqrstuvwxyz");
    producerThreadTwo = makeConsumerProducerThread(producerHandler, stack, "P2", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    consumerThreadOne = makeConsumerProducerThread(consumerHandler, stack, "C1", NULL);
    pthread_create(&threadControl, NULL, control, NULL);

    pthread_join(producerThreadOne->thread, NULL);
    pthread_join(producerThreadTwo->thread, NULL);
    pthread_join(consumerThreadOne->thread, NULL);

    pthread_mutex_destroy(stack->fifo);

    return EXIT_SUCCESS;
}

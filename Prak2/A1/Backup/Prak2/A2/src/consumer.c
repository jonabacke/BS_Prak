

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "consumer.h"

void consumer(FIFOStack *stack)
{
    char result;
    while (1)
    {
        void cancelDisable();
#ifdef condition
        mutex_lock(stack->consumerQueue);
        while (queueConsumer_empty(stack))
        {
            cond_wait(stack->nonEmptyQueueConsumer, stack->consumerQueue);
        }
        stack->next_outconsumerQueue = stack_incr(stack, stack->next_outconsumerQueue);
        mutex_unlock(stack->consumerQueue);
        cond_signal(stack->nonFull);
#else
        semaphore_wait(stack->itemsConsumer);
        mutex_lock(stack->consumerQueue);
        stack->next_outconsumerQueue = stack_incr(stack, stack->next_outconsumerQueue);
        mutex_unlock(stack->consumerQueue);
        semaphore_post(stack->spacesConsuemr);
#endif
#ifdef condition
        mutex_lock(stack->fifo);
        while (stack_empty(stack))
        {
            cond_wait(stack->nonEmpty, stack->fifo);
        }
        result = stack->array[stack->next_out];
        stack->next_out = stack_incr(stack, stack->next_out);
        mutex_unlock(stack->fifo);
        cond_signal(stack->nonFull);
#else
        semaphore_wait(stack->items);
        mutex_lock(stack->fifo);
        result = stack->array[stack->next_out];
        stack->next_out = stack_incr(stack, stack->next_out);
        mutex_unlock(stack->fifo);
        semaphore_post(stack->spaces);
#endif
        cancelEnable();
    }
}
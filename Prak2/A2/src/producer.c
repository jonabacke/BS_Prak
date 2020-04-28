#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "producer.h"

void producer(FIFOStack *stack)
{
    char value;
    while (1)
    {
        void cancelDisable();
#ifdef condition
        mutex_lock(stack->producerQueue);
        while (stack_fullProducer(stack))
        {
            cond_wait(stack->nonFullQueueProducer, stack->producerQueue);
        }
        // take value
        stack->next_inproducerQueue = stack_incr(stack, stack->next_inproducerQueue);
        mutex_unlock(stack->producerQueue);
        cond_signal(stack->nonFullQueueProducer);
#else
        semaphore_wait(stack->spaces);
        mutex_lock(stack->fifo);
        stack->array[stack->next_in] = value;
        stack->next_in = stack_incr(stack, stack->next_in);
        mutex_unlock(stack->fifo);
        semaphore_post(stack->items);
#endif // cond
#ifdef condition
        mutex_lock(stack->fifo);
        while (stack_full(stack))
        {
            cond_wait(stack->nonFull, stack->fifo);
        }
        stack->array[stack->next_in] = value;
        stack->next_in = stack_incr(stack, stack->next_in);
        mutex_unlock(stack->fifo);
        cond_signal(stack->nonFull);
#else
        semaphore_wait(stack->spaces);
        mutex_lock(stack->fifo);
        stack->array[stack->next_in] = value;
        stack->next_in = stack_incr(stack, stack->next_in);
        mutex_unlock(stack->fifo);
        semaphore_post(stack->items);
#endif // cond
        cancelEnable();
    }
}
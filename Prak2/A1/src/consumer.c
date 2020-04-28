

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "consumer.h"

void *consumerHandler(CPThread *thread)
{
    while (1)
    {
        printf("Consumer %s nimmt %c aus Puffer \n", thread->name, consumer(thread->stack, thread));
        sleep(sleepConsumerTimer);
    }
}

int consumer(FIFOStack *stack, CPThread *thread)
{
    char result;
    void cancelDisable();
    mutex_lock(thread->pause);
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
    mutex_unlock(thread->pause);
    cancelEnable();
    return result;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "producer.h"



void *producerHandler(CPThread *thread)
{
    while (1)
    {
        for (int i = 0; i < sizeof(thread->alphabet); i++)
        {
            producer(thread->stack, thread->alphabet[i], thread);
            printf("%s schreibt %c in Puffer \n", thread->name, thread->alphabet[i]);
            sleep(sleepProducerTimer);
        }
    }
}


void producer(FIFOStack *stack, char value, CPThread *thread)
{
    void cancelDisable();
    mutex_lock(thread->pause);
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
    mutex_unlock(thread->pause);
    cancelEnable();
}
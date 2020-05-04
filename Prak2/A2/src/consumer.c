

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "consumer.h"

#define consumerSleep 2

char consumerTask(FIFOStack *stack)
{

        char result;
#ifdef condition
        mutex_lock(stack->fifo);
        while (stack_empty(stack))
        {
                cond_wait(stack->nonEmpty, stack->fifo);
        }
        result = stack->array[stack->next_out];
        stack->next_out = stack_incr(stack->length, stack->next_out);
        mutex_unlock(stack->fifo);
        cond_signal(stack->nonFull);
#else
        semaphore_wait(stack->items);
        mutex_lock(stack->fifo);
        result = stack->array[stack->next_out];
        stack->next_out = stack_incr(stack->length, stack->next_out);
        mutex_unlock(stack->fifo);
        semaphore_post(stack->spaces);
#endif
        printf("%c wurde aus dem Stack geholt\n", result);
        return result;
}

void *consumer(CPThread *thread)
{
        struct TaskHeader header;
        while (1)
        {
                void cancelDisable();
#ifdef condition
                mutex_lock(thread->queue->queue);
                while (queue_full(thread->queue))
                {
                        cond_wait(thread->queue->nonFull, thread->queue->queue);
                }
                receiveFromTaskQueue(thread->queue->schlange, &header, &(thread->value), sizeof(char));
                thread->queue->next_in = stack_incr(thread->queue->length, thread->queue->next_in);
                mutex_unlock(thread->queue->queue);
                cond_signal(thread->queue->nonFull);
#else
                semaphore_wait(thread->queue->spaces);
                mutex_lock(thread->queue->queue);
                receiveFromTaskQueue(thread->queue->schlange, &header, &(thread->value), sizeof(char));
                thread->queue->next_in = stack_incr(thread->queue->length, thread->queue->next_in);
                mutex_unlock(thread->queue->queue);
                semaphore_post(thread->queue->items);
#endif // cond
                cancelEnable();
                (*header.routineForTask)(thread->stack);
                sleep(consumerSleep);
        }
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "producer.h"

#define producerSleep 3

void producerTask(CPThread *thread)
{
#ifdef condition
        mutex_lock(thread->stack->fifo);
        while (stack_full(thread->stack))
        {
                cond_wait(thread->stack->nonFull, thread->stack->fifo);
        }
        thread->stack->array[thread->stack->next_in] = thread->value;
        thread->stack->next_in = stack_incr(thread->stack->length, thread->stack->next_in);
        mutex_unlock(thread->stack->fifo);
        cond_signal(thread->stack->nonFull);
#else
        semaphore_wait(thread->stack->spaces);
        mutex_lock(thread->stack->fifo);
        thread->stack->array[thread->stack->next_in] = value;
        thread->stack->next_in = stack_incr(thread->stack, thread->stack->next_in);
        mutex_unlock(thread->stack->fifo);
        semaphore_post(thread->stack->items);
#endif // cond
}

void *producer(CPThread *thread)
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
                printf("hallo \n");
#else
                semaphore_wait(thread->queue->spaces);
                mutex_lock(thread->Queue);
                receiveFromTaskQueue(thread->queue->schlange, &header, &(thread->value), sizeof(char));
                thread->queue->next_in = stack_incr(thread->queue->length, thread->queue->next_in);
                mutex_unlock(thread->queue->queue);
                semaphore_post(thread->queue->items);
#endif // cond
                cancelEnable();
                (*header.routineForTask)(thread);

                sleep(producerSleep);
        }
}
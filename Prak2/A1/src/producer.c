#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>
#include"general.h"
#include "producer.h"
#include "mutex.h"


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

    mutex_lock(thread->pause);
    //cancelDisable();
#ifdef condition
    mutex_lock(stack->fifo);
    pthread_cleanup_push(cleanup_handler, thread);
    while (stack_full(stack))
    {
        cond_wait(stack->nonFull, stack->fifo);
    }
    stack->array[stack->next_in] = value;
    stack->next_in = stack_incr(stack, stack->next_in);
    pthread_cleanup_pop(1);
    //mutex_unlock(stack->fifo);
    cond_signal(stack->nonFull);
#else
    cancelDisable();
    semaphore_wait(stack->spaces);
    mutex_lock(stack->fifo);
    stack->array[stack->next_in] = value;
    stack->next_in = stack_incr(stack, stack->next_in);
    mutex_unlock(stack->fifo);
    semaphore_post(stack->items);
    cancelEnable();
#endif // cond
    //cancelEnable();
    mutex_unlock(thread->pause);

}

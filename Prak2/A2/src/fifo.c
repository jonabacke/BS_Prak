#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "fifo.h"

FIFOStack *make_stack(int length, int lengthConsumer, int lengthProducer)
{
    FIFOStack *stack = (FIFOStack *)check_malloc(sizeof(FIFOStack));
    stack->length = length;
    stack->lengthConsumer = lengthConsumer;
    stack->lengthProducer = lengthProducer;
    stack->array = (char *)check_malloc(length * sizeof(char));
    stack->next_in = 0;
    stack->next_out = 0;
    stack->next_inproducerQueue = 0;
    stack->next_outproducerQueue = 0;
    stack->next_inconsumerQueue = 0;
    stack->next_outconsumerQueue = 0;
    stack->fifo = make_mutex();
    stack->consumerQueue = make_mutex();
    stack->producerQueue = make_mutex();
#ifdef condition
    stack->nonEmpty = make_cond();
    stack->nonFull = make_cond();
    stack->nonEmptyQueueConsumer = make_cond();
    stack->nonFullQueueConsumer = make_cond();
    stack->nonEmptyQueueProducer = make_cond();
    stack->nonFullQueueProducer = make_cond();
#else
    stack->items = make_semaphore(0);
    stack->spaces = make_semaphore(length - 1);
    stack->itemsQueueProducer = make_semaphore(0);
    stack->spacesQueueProducer = make_semaphore(length - 1);
    stack->itemsQueueConsumer = make_semaphore(0);
    stack->spacesQueueConsumer = make_semaphore(length - 1);
#endif // cond
    return stack;
}


int stack_incr(FIFOStack *stack, int next)
{
    return (next + 1) % stack->length;
}

int stack_empty(FIFOStack *stack)
{
    return (stack->next_in == stack->next_out);
}


int stack_full(FIFOStack *stack)
{
    return (stack_incr(stack, stack->next_in) == stack->next_out);
}

int stack_incrConsumer(FIFOStack *stack, int next)
{
    return (next + 1) % stack->lengthConsumer;
}

int stack_emptyConsumer(FIFOStack *stack)
{
    return (stack->next_inconsumerQueue == stack->next_outconsumerQueue);
}


int stack_fullConsumer(FIFOStack *stack)
{
    return (stack_incr(stack, stack->next_inconsumerQueue) == stack->next_outconsumerQueue);
}

int stack_incrProducer(FIFOStack *stack, int next)
{
    return (next + 1) % stack->lengthProducer;
}

int stack_emptyProducer(FIFOStack *stack)
{
    return (stack->next_inproducerQueue == stack->next_outproducerQueue);
}


int stack_fullProducer(FIFOStack *stack)
{
    return (stack_incr(stack, stack->next_inproducerQueue) == stack->next_outproducerQueue);
}
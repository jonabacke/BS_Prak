#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>

#include "fifo.h"

FIFOStack *make_stack(int length)
{
    FIFOStack *stack = (FIFOStack *)check_malloc(sizeof(FIFOStack));
    stack->length = length;
    stack->array = (char *)check_malloc(length * sizeof(char));
    stack->next_in = 0;
    stack->next_out = 0;
    stack->fifo = make_mutex();
#ifdef condition
    stack->nonEmpty = make_cond();
    stack->nonFull = make_cond();
#else
    stack->items = make_semaphore(0);
    stack->spaces = make_semaphore(length - 1);
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


#include <stdio.h>
#include <stddef.h>



#include "taskQueue.h"
#include "getcharTimeout.h"
#include "thread.h"

pthread_t initProducerQueue(void *stack);
void *runProducerQueue(Queue *queue);


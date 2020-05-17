

#include <stdio.h>
#include <stddef.h>



#include "taskQueue.h"
#include "getcharTimeout.h"
#include "thread.h"

Queue *initProducerQueue(char *name);
void *runProducerQueue(Queue *queue);


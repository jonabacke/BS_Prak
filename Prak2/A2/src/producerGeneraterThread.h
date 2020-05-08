

#include <stdio.h>
#include <stddef.h>



#include "taskQueue.h"
#include "getcharTimeout.h"
#include "thread.h"

Queue *initProducerQueue(void);
void *runProducerQueue(Queue *queue);


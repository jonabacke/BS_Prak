

#include <stdio.h>
#include <stddef.h>



#include "taskQueue.h"
#include "getcharTimeout.h"
#include "thread.h"

pthread_t initProducerQueue(void);
void *runProducerQueue(Queue *queue);


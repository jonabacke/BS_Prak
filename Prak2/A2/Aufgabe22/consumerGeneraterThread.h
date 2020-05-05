

#include <stdio.h>
#include <stddef.h>



#include "taskQueue.h"
#include "getcharTimeout.h"
#include "thread.h"

#define queueSize 10
#define sizeConsumerQueue 10


pthread_t initConsumerQueue(void);

void *runConsumerQueue(Queue *queue);
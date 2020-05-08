

#include <stdio.h>
#include <stddef.h>



#include "taskQueue.h"
#include "getcharTimeout.h"
#include "queue.h"

#define queueSize 10
#define sizeConsumerQueue 10


Queue *initConsumerQueue(void);

void *runConsumerQueue(Queue *queue);

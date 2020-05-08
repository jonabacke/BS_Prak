
#include "sem.h"
#include "conditionVariable.h"




void readFromQueue(Queue *queue, char *argBuf, struct TaskHeader *header);

void writeIntoQueue(Queue *queue, const char *arg, struct TaskHeader header);


int is_queue_full(Queue *queue);
int is_queue_empty(Queue *queue);
int queuePointer_incr(int next);

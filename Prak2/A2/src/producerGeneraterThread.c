
#include "producerGeneraterThread.h"

#define queueSize 10

typedef struct {
   void (*routineForTask)(void *arg); 	//! Pointer of the function that implements the task
   unsigned int argSize;				//! Size of the structure containing the arguments
}TaskHeader;

TaskHeader* initTaskHeader() 
{
    TaskHeader *task = (TaskHeader *)check_malloc(sizeof(TaskHeader));
    task ->argSize = sizeof(char);
    task ->routineForTask = producer;
    return task;
}


void initProducerQueue() 
{
    mqd_t producerQueue = createTaskQueue("/producer", queueSize, sizeof(char));
    TaskHeader *task = initTaskHeader();
    while (1)
    {
        char arg = getcharTimeout(1);
        sendToTaskQueue(producerQueue, task, &arg, true);
        printf("%c soll in den Stack gelegt werden");
    }
    
}
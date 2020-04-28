
#include "consumerGeneraterThread.h"

#define queueSize 10

// TaskHeader *initTaskHeader() 
// {
//     TaskHeader *task = (TaskHeader *)check_malloc(sizeof(TaskHeader));
//     task ->argSize = sizeof(char);
//     task ->routineForTask = producer;
//     return task;
// }


void initConsumerQueue() 
{
    mqd_t consumerQueue = createTaskQueue("/consumer", queueSize, sizeof(char));
    // TaskHeader task = *initTaskHeader();
    char result;
    while (1)
    {
        sleep(1);
        sendToTaskQueue(consumerQueue, NULL, &result, true);
        printf("%c aus dem Stack geholt\n", result);
    }
    
}
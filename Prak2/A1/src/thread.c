/* ============================================================================
 * Name        : thread.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides a function to initialize producer or consumer threads.
 * ============================================================================
 */


#include "thread.h"


/*Declaration of the threads*/
CPThread *producerThread_1;
CPThread *producerThread_2;
CPThread *consumerThread;
pthread_t controlThread;



/* ============================================================================
*  @brief   Creates a thread.
*  @param   'thread'        The thread to create (producer/consumer/control).
*  @param   'function'      The function to work with.
*  @param   'fifoBuffer'    The buffer to work with.
*/
void make_thread(pthread_t *thread, void *function, void *fifoBuffer)
{
    int n = pthread_create(thread, NULL, function, fifoBuffer);
    HANDLE_ERR(n);
}



/* ============================================================================
*  @brief   Initializes a producer or a consumer thread.
*  @param   'function'      The function to work with (producerHandler/consumerHandler/control).
*  @param   'fifoBuffer'    The buffer to read from or write on.
*  @param   'name'          Name of the Thread
*/
CPThread *makeConsumerProducerThread(void *function, FIFOBuffer *fifoBuffer, char *name)
{
    CPThread *thread = check_malloc(sizeof(CPThread));
    make_thread(&(thread->thread), function, thread);
    thread->pauseMutex = make_mutex();
    thread->flag = 1;
    thread->name = name;
    thread->fifoBuffer = fifoBuffer;
    return thread;    
}



/* ============================================================================
*  @brief   Set cancel state (enable) with error handling.
*/
void cancelEnable()
{
    int scs = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    HANDLE_ERR(scs);
}



/* ============================================================================
*  @brief   Set cancel state (disable) with error handling.
*/
void cancelDisable()
{
    int scs = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    HANDLE_ERR(scs);
}



/* ============================================================================
*  @brief   Function for allocating storage with error handling.
*/
void *check_malloc(int size)
{
    void *p = malloc(size);
    if (p == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    return p;
}





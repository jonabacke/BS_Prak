/* ============================================================================
 * Name        : thread.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides a function to initialize producer or consumer threads.
 * ============================================================================
 */


#include "thread.h"

#include <pthread.h>




/* ============================================================================
*  @brief   Creates a thread.
*  @param   'thread'        The thread to create (producer/consumer/control).
*  @param   'function'      The function to work with.
*  @param   'fifoBuffer'    The buffer to work with.
*/
void make_thread(pthread_t thread, void *function, void *arg)
{
    printf("generate Thread \n");
    int n = pthread_create(&thread, NULL, function, arg);
    HANDLE_ERR(n);
}



/* ============================================================================
*  @brief   Initializes a producer or a consumer thread.
*  @param   'function'      The function to work with (producerHandler/consumerHandler/control).
*  @param   'fifoBuffer'    The buffer to read from or write on.
*  @param   'name'          Name of the Thread
*/
pthread_t makeConsumerProducerThread(void *function,  Queue *queue)
{
    pthread_t thread;
    make_thread(thread, function, queue);
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





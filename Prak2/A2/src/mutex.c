/* ============================================================================
 * Name        : mutex.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and handle mutexes.
 * ============================================================================
 */


#include "mutex.h"

/* ============================================================================
*  @brief   This function creates a Mutex.
*  @return  'mutex'     Returns a Mutex
*/
Mutex *make_mutex(void)
{
    Mutex *mutex = check_malloc(sizeof(Mutex));
    int n = pthread_mutex_init(mutex, NULL);
    HANDLE_ERR(n);
    return mutex;
}



/* ============================================================================
*  @brief   This function locks a Mutex - with error handling.
*/
void mutex_lock(Mutex *mutex)
{
    int n = pthread_mutex_lock(mutex);
    HANDLE_ERR(n);
}



/* ============================================================================
*  @brief   This function unlocks a Mutex - with error handling.
*/
void mutex_unlock(Mutex *mutex)
{
    int n = pthread_mutex_unlock(mutex);
    HANDLE_ERR(n);
}



/* ============================================================================
*  @brief   This function provides the cleanup handling for a Mutex 
*           in case of a thread canceling while the buffer is locked.
*/
void cleanup_handler(void *p)
{	
	mutex_unlock(p);
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
        EXIT_FAILURE;
    }
    return p;
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




/* ============================================================================
 * Name        : mutex.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and handle mutexes.
 * ============================================================================
 */


#include "mutex.h"
#include "fifo.h"


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







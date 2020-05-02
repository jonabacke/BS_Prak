/* ============================================================================
 * Name        : mutex.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and handle mutexes.
 * ============================================================================
 */


#include <sys/sem.h>
#include "mutex.h"


Mutex *make_mutex(void)
{
    Mutex *mutex = check_malloc(sizeof(Mutex));
    int n = pthread_mutex_init(mutex, NULL);
    HANDLE_ERR(n);
    return mutex;
}


void mutex_lock(Mutex *mutex)
{
    int n = pthread_mutex_lock(mutex);
    HANDLE_ERR(n);
}


void mutex_unlock(Mutex *mutex)
{
    int n = pthread_mutex_unlock(mutex);
    if (n != 0)
    HANDLE_ERR(n);
}


void cleanup_handler(CPThread *thread)
{
	//mutex_unlock(thread->stack->fifo);
	mutex_unlock(thread->pause);
}

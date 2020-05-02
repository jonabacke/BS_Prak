/* ============================================================================
 * Name        : mutex.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and handle mutexes.
 * ============================================================================
 */


#ifndef _MUTEX_H
#define _MUTEX_H


#include "general.h"
#include "thread.h"


typedef pthread_mutex_t Mutex;


Mutex *make_mutex(void);


void mutex_lock(Mutex *mutex);


void mutex_unlock(Mutex *mutex);


void cleanup_handler(CPThread *thread);


#endif /*_MUTEX_H*/

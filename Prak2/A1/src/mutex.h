/* ============================================================================
 * Name        : mutex.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and handle mutexes.
 * ============================================================================
 */


#ifndef _MUTEX_H
#define _MUTEX_H


#include "thread.h"
#include "general.h"


typedef pthread_mutex_t Mutex;



/* ============================================================================
*  @brief   This function creates a Mutex.
*  @return  'mutex'     Returns a Mutex
*/
Mutex *make_mutex(void);



/* ============================================================================
*  @brief   This function locks a Mutex - with error handling.
*/
void mutex_lock(Mutex *mutex);



/* ============================================================================
*  @brief   This function unlocks a Mutex - with error handling.
*/
void mutex_unlock(Mutex *mutex);



/* ============================================================================
*  @brief   This function provides the cleanup handling for a thread Mutex 
*           in case of a thread canceling while the buffer is locked.
*/
void cleanup_handler(void *p);



#endif /*_MUTEX_H*/

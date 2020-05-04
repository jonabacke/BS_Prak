/* ============================================================================
 * Name        : conditionVariable.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and use condition variables.
 * ============================================================================
 */

#ifndef _CONDITIONVARIABLE_H
#define _CONDITIONVARIABLE_H


#include "thread.h"
#include "general.h"


typedef pthread_cond_t Cond;
typedef pthread_mutex_t Mutex;



/* ============================================================================
*  @brief   Makes a thread wait for a condition variable to be signaled.
*           (Function is a cancellation point.)
*  @param   cond    Condition that makes a thread wait
*  @param   mutex   Mutex to lock the section
*/
void cond_wait(Cond *cond, Mutex *mutex);



/* ============================================================================
*  @brief   Makes a thread wake up for a condition variable to be signaled.
*  @param   cond    Condition that makes a thread wake up.
*/
void cond_signal(Cond *cond);



/* ============================================================================
*  @brief   Initializes a condition variable.
*  @return  Returns a conditional variable
*/
Cond *make_cond(void);



#endif /*_CONDITIONVARIABLE_H*/
/* ============================================================================
 * Name        : conditionVariable.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to initialize and use condition variables.
 * ============================================================================
 */


#include "conditionVariable.h"


/* @brief   Makes a thread wait for a condition variable to be signaled.
*           (Function is a cancellation point.)
*  @param   cond    Condition that makes a thread wait
*  @param   mutex   Mutex to lock the section
*/
void cond_wait(Cond *cond, Mutex *mutex)
{
    int n = pthread_cond_wait(cond, mutex);
    HANDLE_ERR(n);
}


/* @brief   Makes a thread wake up for a condition variable to be signaled.
*  @param   cond    Condition that makes a thread wake up.
*/
void cond_signal(Cond *cond)
{
    int n = pthread_cond_signal(cond);
    HANDLE_ERR(n);
}


/* @brief   Initializes a condition variable.
*/
Cond *make_cond(void)
{
    Cond *cond = check_malloc(sizeof(Cond));
    int n = pthread_cond_init(cond, NULL);
    HANDLE_ERR(n);

    return cond;
}






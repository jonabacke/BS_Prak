/* ============================================================================
 * Name        : sem.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to create and use semaphores.
 * ============================================================================
 */


#ifndef _SEM_H
#define _SEM_H


#include "thread.h"
#include "general.h"



typedef sem_t Semaphore;



/* ============================================================================
*  @brief   Initialize a semaphore with a certain value.
*  @param   'value'     to initialize the semaphore with.
*                       (i.e. buffer size, elements in buffer)
*  @return  'sem'       Return an initialized Semaphore.
*/
Semaphore *make_semaphore(int value);



/* ============================================================================
*  @brief   Waiting for semaphore being posted.
*           (This function is a cancellation point.)
*  @param   sem  is the semaphore
*/
void semaphore_wait(Semaphore *sem);



/* ============================================================================
*  @brief   Post the semaphore.
*  @param   sem  is the semaphore
*/
void semaphore_post(Semaphore *sem);



#endif /*_SEM_H*/
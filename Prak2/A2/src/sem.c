/* ============================================================================
 * Name        : sem.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides functions to create and use semaphores.
 * ============================================================================
 */


#include "sem.h"



/* ============================================================================
*  @brief   Initialize a semaphore with a certain value.
*  @param   'value'     to initialize the semaphore with.
*                       (i.e. buffer size, elements in buffer)
*  @return  'sem'       Return an initialized Semaphore.
*/
Semaphore *make_semaphore(int value)
{
    Semaphore *sem = check_malloc(sizeof(Semaphore));
    int n = sem_init(sem, 0, value);
    HANDLE_ERR(n);
    return sem;
}



/* ============================================================================
*  @brief   Waiting for semaphore being posted.
*           (This function is a cancellation point.)
*  @param   sem  is the semaphore
*/
void semaphore_wait(Semaphore *sem)
{
    int n = sem_wait(sem);
    HANDLE_ERR(n);
}



/* ============================================================================
*  @brief   Post the semaphore.
*  @param   sem  is the semaphore
*/
void semaphore_post(Semaphore *sem)
{
    int n = sem_post(sem);
    HANDLE_ERR(n);
}
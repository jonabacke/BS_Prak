/* ============================================================================
 * Name        : thread.c
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides a function to initialize producer or consumer threads.
 * ============================================================================
 */


#include "thread.h"


/* @brief   Initializes a producer or a consumer thread.
*  @param   fifoBuffer  The buffer to read from or write on.
*  @param   name        Name of the Thread
*/
CPThread *makeConsumerProducerThread(FIFOBuffer *fifoBuffer, char *name)
{
    CPThread *thread = check_malloc(sizeof(CPThread));
    thread->pause = make_mutex();
    thread->flag = 1;
    thread->name = name;
    thread->fifoBuffer = fifoBuffer;
    return thread;    
}




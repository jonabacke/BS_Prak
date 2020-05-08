/* ============================================================================
 * Name        : thread.h
 * Author      : Jonathan Backes, Tobias Hardjowirogo
 * Version     : 1.1
 * Description : This file provides a function to create producer or consumer threads.
 * ============================================================================
 */


#ifndef _THREAD_H
#define _THREAD_H

#include <stdlib.h>

#include "consumer.h"
#include "producer.h"



/* ============================================================================
*  @brief   Creates a thread.
*  @param   'thread'        The thread to create (producer/consumer/control).
*  @param   'function'      The function to work with (producerHandler/consumerHandler/control).
*  @param   'fifoBuffer'    The buffer to work with.
*/
void make_thread(pthread_t* thread, void *function, void *fifoBuffer);



/* ============================================================================
*  @brief   Initializes a producer or a consumer thread.
*  @param   'function'      The function to work with (producerHandler/consumerHandler/control).
*  @param   'fifoBuffer'    The buffer to read from or write on.
*  @param   'name'          Name of the Thread
*/
//pthread_t makeConsumerProducerThread(void *function, Queue *queue);






#endif /*_THREAD_H*/





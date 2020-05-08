#ifndef CONTROLER_H
#define CONTROLER_H


#include <ctype.h> //tolower
#include "mutex.h"
#include "consumerGeneraterThread.h"
#include "producerGeneraterThread.h"
#include "producer.h"
#include "consumer.h"



/* ============================================================================
*  @brief   Function used by the control thread. Reads key input, can pause 
*           and terminate the created threads.
*/
void *control(void *not_used);



/* ============================================================================
*  @brief   Toggles (pauses) the producer or consumer threads.
*  @param   Thread to be toggled.
*/
void toggleThread(Queue *queue);



/* ============================================================================
*  @brief   Prints all available commands when 'h' key is pressed.
*/
void printCommands();



/* ============================================================================
*  @brief   Terminates the producer and consumer threads.
*/
void cancelAll();

void deleteAll();

#endif /*_CONTROLER_H*/

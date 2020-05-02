/* ============================================================================
 * @file        : controler.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the control thread which can control and 
 *                terminate the threads.
 * ============================================================================
 */


#ifndef _CONTROLER_H
#define _CONTROLER_H


#include "general.h"


/* @brief   Reads key input, can pause and terminate the threads.
*/
void *control(void *not_used);


/* @brief   Toggles (pauses) the producer or consumer threads.
*  @param   Thread to be toggled.
*/
void toggleThread(CPThread *thread);


/* @brief   Prints all available commands when 'h' key is pressed.
*/
void printCommands();


/* @brief   Terminates the producer and consumer threads.
*/
void cancelAll();


#endif /*_CONTROLER_H*/

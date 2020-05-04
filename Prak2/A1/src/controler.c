/* ============================================================================
 * @file        : controler.c
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the control thread which can control and 
 *                terminate the threads.
 * ============================================================================
 */


#include "controler.h"



/* ============================================================================
*  @brief   Function used by the control thread. Reads key input, can pause 
*           and terminate the created threads.
*/
void *control(void *not_used)
{
    while (1)
    {   
        /*READING AND EVALUATING KEY INPUT*/
        char order = getchar();

        switch (tolower(order))
        {
        case '1':
            toggleThread(producerThread_1);
            printf("--- toggle Producer_1\n");
            break;

        case '2':
            toggleThread(producerThread_2);
            printf("--- toggle Producer_2\n");
            break;

        case 'c':
            toggleThread(consumerThread);
            printf("--- toggle consumer\n");
            break;

        case 'q':
            printf("--- Programm wird beendet.\n");
            cancelAll();
            break;

        case 'h':
            printCommands();
            break;

        default:
            break;
        }
    }
}



/* ============================================================================
*  @brief   Toggles (pauses) the producer or consumer threads.
*  @param   Thread to be toggled.
*/
void toggleThread(CPThread *thread)
{
    if (thread->flag)
    {
        thread->flag = TURN_OFF;
        mutex_lock(thread->pauseMutex);
    }
    else
    {
        thread->flag = TURN_ON;
        mutex_unlock(thread->pauseMutex);
    }
}



/* ============================================================================
*  @brief   Prints all available commands when 'h' key is pressed.
*/
void printCommands()
{
    printf( "\n\n -----------------------------------\n" 
            "Commands:\n"
            "1\t start/stop 'Producer_1'\n"
            "2\t start/stop 'Producer_2'\n" 
            "c/C\t start/stop 'Consumer'\n"
            "q/Q\t terminate the system\n" 
            "h\t print commands\n"
            "------------------------------------\n\n\n ");
}



/* ============================================================================
*  @brief   Terminates the producer and consumer threads.
*/
void cancelAll() 
{
	printf("cancelAll wird betreten \n");
    int tc1 = pthread_cancel(producerThread_1->thread);
    HANDLE_ERR(tc1);
    int tc2 = pthread_cancel(producerThread_2->thread);
    HANDLE_ERR(tc2);
    int tc3 = pthread_cancel(consumerThread->thread);
    HANDLE_ERR(tc3);
    toggleThread(producerThread_1);
    toggleThread(producerThread_2);
    toggleThread(consumerThread);
}





/* ============================================================================
 * @file        : controler.h
 * @author      : Jonathan Backes, Tobias Hardjowirogo
 * @version     : 1.1
 * @brief       : This file provides the control thread which can control and 
 *                terminate the threads.
 * ============================================================================
 */


#include "controler.h"


/* @brief   Reads key input, can pause and terminate the threads.
*/
void *control(void *not_used)
{
    /*READING AND EVALUATING INPUT*/
    char order = getchar();
    while (1)
    {
        switch (tolower(order))
        {
        case '1':
            toggleThread(Producer_1);
            printf("--- toggle Producer_1\n");
            break;

        case '2':
            toggleThread(Producer_2);
            printf("--- toggle Producer_2\n");
            break;

        case 'c':
            toggleThread(Consumer);
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


/* @brief   Toggles (pauses) the producer or consumer threads.
*  @param   Thread to be toggled.
*/
void toggleThread(CPThread *thread)
{
    if (thread->flag)
    {
        thread->flag = TURN_OFF;
        mutex_lock(thread->pause);
    }
    else
    {
        thread->flag = TURN_ON;
        mutex_unlock(thread->pause);
    }
}


/* @brief   Prints all available commands when 'h' key is pressed.
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


/* @brief   Terminates the producer and consumer threads.
*/
void cancelAll() 
{
	printf("cancelAll wird betreten");
    int tc1 = pthread_cancel(Producer_1->thread);
    HANDLE_ERR(tc1);
    int tc2 = pthread_cancel(Producer_2->thread);
    HANDLE_ERR(tc2);
    int tc3 = pthread_cancel(Consumer->thread);
    HANDLE_ERR(tc3);
}

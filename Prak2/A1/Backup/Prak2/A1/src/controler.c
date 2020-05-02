#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>
#include <ctype.h>
#include "myError.h"

#include "controler.h"


void *control(void *not_used)
{
    char eingabe;
    while (1)
    {
           // sleep(3);
        // eingabe einlesen
        scanf("%c", &eingabe);
        // reagieren
        switch (tolower(eingabe))
        {
        case '1':
            toggleThread(producerThreadOne);
            printf("--- toggle Producer_1\n");
            break;

        case '2':
            toggleThread(producerThreadTwo);
            printf("--- toggle Producer_2\n");
            break;

        case 'c':
            toggleThread(consumerThreadOne);
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
void toggleThread(CPThread *stack)
{
    if (stack->flag)
    {
        stack->flag = TURN_OFF;
        mutex_lock(stack->pause);
    }
    else
    {
        stack->flag = TURN_ON;
        mutex_unlock(stack->pause);
    }
}



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



void cancelAll() 
{
	printf("cancelAll wird betreten");
    int tc1 = pthread_cancel(producerThreadOne->thread);
    HANDLE_ERR(tc1);
    int tc2 = pthread_cancel(producerThreadTwo->thread);
    HANDLE_ERR(tc2);
    int tc3 = pthread_cancel(consumerThreadOne->thread);
    HANDLE_ERR(tc3);
}

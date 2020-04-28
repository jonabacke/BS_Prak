#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <malloc.h>
#include <errno.h>
#include <ctype.h>

#include "controler.h"


void *control(void *not_used)
{
    char eingabe;
    while (1)
    {
            sleep(3);
        // eingabe einlesen
        scanf("%c", &eingabe);
        // reagieren
        switch (tolower(eingabe))
        {
        case '1':
            toggleThread(producerThreadOne);
            break;

        case '2':
            toggleThread(producerThreadTwo);
            break;

        case 'c':
            toggleThread(consumerThreadOne);
            break;

        case 'q':
            printf("beende System");
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
        stack->flag = !(stack->flag);
        mutex_lock(stack->pause);
    }
    else
    {
        stack->flag = !(stack->flag);
        mutex_unlock(stack->pause);
    }
}

void printCommands()
{
    printf(
        "\n\n -----------------------------------\n Commands:\n 1\t start / stop 'Producer_1'\n 2\t start / stop 'Producer_2'\n c / C\t start / stop 'Consumer'\n q / Q\t terminate the system\n h\t print commands\n------------------------------------\n\n\n ");
}

void cancelAll() 
{
    pthread_cancel(producerThreadOne->thread);
    pthread_cancel(producerThreadTwo->thread);
    pthread_cancel(consumerThreadOne->thread);
    // pthread_cancel(threadControl);
}
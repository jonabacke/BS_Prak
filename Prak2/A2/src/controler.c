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
    int flagProducer = 1;
    int flagConsumer = 1;
    while (1)
    {
        printf("hi9 \n");
        // eingabe einlesen
        scanf("%c", &eingabe);
        // reagieren
        switch (tolower(eingabe))
        {
        case 'p':
            flagProducer = toggleThread(generateProducerMutex, flagProducer);
            break;

        case 'c':
            flagConsumer = toggleThread(generateConsumerMutex, flagConsumer);
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
int toggleThread(Mutex *mutex, int flag)
{
    if (flag)
    {
        flag = 0;
        mutex_lock(mutex);
    }
    else
    {
        flag = 1;
        mutex_unlock(mutex);
    }
    return flag;
}

void printCommands()
{
    printf(
        "\n\n -----------------------------------\n Commands:\n 1\t start / stop 'Producer_1'\n 2\t start / stop 'Producer_2'\n c / C\t start / stop 'Consumer'\n q / Q\t terminate the system\n h\t print commands\n------------------------------------\n\n\n ");
}

void cancelAll()
{
    pthread_cancel(threadControl);
    pthread_cancel(producerQueue);
    pthread_cancel(consumerQueue);
    toggleThread(generateProducerMutex, 0);
    toggleThread(generateProducerMutex, 0);
}
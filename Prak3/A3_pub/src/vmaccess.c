/**
 * @file vmaccess.c
 * @author Prof. Dr. Wolfgang Fohl, HAW Hamburg
 * @date 2010
 * @brief The access functions to virtual memory.
 */


#include "vmaccess.h"
#include <sys/ipc.h>
#include <sys/shm.h>


#include "syncdataexchange.h"
#include "vmem.h"
#include "debug.h"


/*
 * static variables
 */

static struct vmem_struct *vmem = NULL; //!< Reference to virtual memory


/**
 * The progression of time is simulated by the counter g_count, which is incremented by 
 * vmaccess on each memory access. The memory manager will be informed by a command, whenever 
 * a fixed period of time has passed. Hence the memory manager must be informed, whenever 
 * g_count % TIME_WINDOW == 0. 
 * Based on this information, memory manager will update aging information
 */

static int g_count = 0;    //!< global acces counter as quasi-timestamp - will be increment by each memory access
#define TIME_WINDOW   20



static void vmem_init(void) {

    /* Create System V shared memory */

    /* We are only using the shm, don't set the IPC_CREAT flag */

    /* attach shared memory to vmem */

}



static void vmem_put_page_into_mem(int address) {

}



int vmem_read(int address) {

}



void vmem_write(int address, int data) {

}



extern void vmem_close(void) {

}


// EOF

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
    //TODO:

    /* Create System V shared memory */

    /* We are only using the shm, don't set the IPC_CREAT flag */

    /* attach shared memory to vmem */

}



static void vmem_put_page_into_mem(int address) {
    //TODO:
    /*
    Speicherverwaltung auffordern, Seite aus dem <pagefile> in den Hauptspeicher zu laden:

    struct msg msg;
    msg->cmd = CMD_PAGEFAULT;
    msg->value = address;
    msg->g_count = ?;       //Der g_count modelliert die aktuelle Zeit, in dem die Anzahl der Speicherzugriffe durch vmaccess gezählt wird.
    msg->ref = ?;           //Fortlaufender Ref-Counter zur Zuordnung zwischen Befehl und Antwort.

    int frameID = find_unused_frame(); //freien Seitenrahmen suchen

    if (frameID == VOID_IDX) {
        - Seite auslagern (FIFO/CLOCK/AGING) und neue einlesen;
    }

    sendMsgToMmanager(msg); //synchdataexchange: Seite aus pagefile -> mainMemory
    */
}



int vmem_read(int address) {
    //TODO:
    /*
    - read und write berechnen aus der virtuellen Speicheradresse die Framenummer und den Offset:

    int seitenNummer = (address & 0x1000) >> 3;
    int offset = address & 0x0111;
    
    if (pt->frame == VOID_IDX) Seitenfehler; // no frame reference in page table
    else {
        int pageFrameNummer = pt[seitenNummer]->frame;
    }

    
    - wenn die benötigte Seite nicht geladen ist 
            -> vmem_put_page_into_mem(address); (pagefile -> mainMemory)
    */
}



void vmem_write(int address, int data) {
    //TODO:
    /*
    - read und write berechnen aus der virtuellen Speicheradresse die Framenummer und den Offset
    - wenn die benötigte Seite nicht geladen ist 
            -> vmem_put_page_into_mem(address); (pagefile -> mainMemory)
    */

}



extern void vmem_close(void) {
    //TODO:

}


// EOF

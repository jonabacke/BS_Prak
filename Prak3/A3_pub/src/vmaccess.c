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

static int g_count = 0; //!< global acces counter as quasi-timestamp - will be increment by each memory access
static int ref = 0;

#define TIME_WINDOW 20

static void vmem_init(void)
{
    //TODO:
    /* Create System V shared memory */
    key_t key = ftok(SHMKEY, SHMPROCID);

    /* We are only using the shm, don't set the IPC_CREAT flag */
    int shm_id = shmget(key, SHMSIZE, 0666);
    TEST_AND_EXIT_ERRNO(shm_id, "shmget error");

    /* attach shared memory to vmem */

    vmem = shmat(shm_id, NULL, 0);
    TEST_AND_EXIT_ERRNO(vmem, "shmat error");
}

static void vmem_put_page_into_mem(int address)
{
    //TODO:
    /* Speicherverwaltung auffordern, Seite aus dem <pagefile> in den Hauptspeicher zu laden: */

    int page = address / VMEM_PAGESIZE;
    int offset = address % VMEM_PAGESIZE;
    struct msg msg;

    /* Prüfen, ob page in einem pageframe ist. */
    if (vmem->pt[page].frame == VOID_IDX) //! Vllt besser PTF_PRESENT checken?
    {
        msg.cmd = CMD_PAGEFAULT; // value gibt die einzulagernde Page mit
        msg.value = address;
        msg.g_count = g_count; //modelliert die aktuelle Zeit, indem die Anzahl der Speicherzugriffe durch vmaccess gezählt wird.
        msg.ref = ref;         //Fortlaufender Ref-Counter zur Zuordnung zwischen Befehl und Antwort.

        sendMsgToMmanager(msg); //synchdataexchange: Seite aus pagefile -> mainMemory
        waitForMsg();
        ref++;
    }

    if (g_count == TIME_WINDOW) //Wenn Zeitintervall abgelaufen ist
    {
        msg.cmd = CMD_TIME_INTER_VAL; //for aging algo
        sendMsgToMmanager(msg);
        g_count = 0; //reset g_count
        waitForMsg();
    }
    g_count++;
}

int vmem_read(int address)
{
    //TODO:
    /*
    - read und write berechnen aus der virtuellen Speicheradresse die Framenummer und den Offset:

    int seitenNummer = (address & 0x1000) >> 3;
    int offset = address & 0x0111;
    
    if (vmem->pt[seitenNummer].frame == VOID_IDX) 
    {
        vmem_put_page_into_mem(address); // Seitenfehler: no frame reference in page table   (pagefile -> mainMemory)
    } 
    else 
    {
        int pageFrameNummer = vmem->pt[seitenNummer].frame;
    }

    */
    if (vmem == NULL)
    {
        vmem_init();
    }

    vmem_put_page_into_mem(address);

    int page = address / VMEM_PAGESIZE;
    int offset = address % VMEM_PAGESIZE;

    int frame = vmem->pt[page].frame;

    return vmem->mainMemory[frame * VMEM_PAGESIZE + offset];
}

void vmem_write(int address, int data)
{
    //TODO:
    /*
    - read und write berechnen aus der virtuellen Speicheradresse die Framenummer und den Offset
    - wenn die benötigte Seite nicht geladen ist 
            -> vmem_put_page_into_mem(address); (pagefile -> mainMemory)
    */
    if (vmem == NULL)
    {
        vmem_init();
    }

    int page = address / VMEM_PAGESIZE;
    int offset = address % VMEM_PAGESIZE;

    vmem_put_page_into_mem(address);

    int frame = vmem->pt[page].frame;

    vmem->mainMemory[frame * VMEM_PAGESIZE + offset] = data;
    vmem->pt[page].flags = vmem->pt[page].flags | PTF_DIRTY | PTF_REF;
}

extern void vmem_close(void)
{
    //TODO:
}

// EOF

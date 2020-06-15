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

/**
 *****************************************************************************************
 *  @brief      This function setup the connection to virtual memory.
 *              The virtual memory has to be created by mmanage.c module.
 *
 *  @return     void
 ****************************************************************************************/
static void vmem_init(void);



/**
 *****************************************************************************************
 *  @brief      This function puts a page into memory (if required). Ref Bit of page table
 *              entry will be updated.
 *              If the time window handle by g_count has reached, the window window message
 *              will be send to the memory manager.
 *              To keep conform with this log files, g_count must be increased before
 *              the time window will be checked.
 *              vmem_read and vmem_write call this function.
 *
 *  @param      address The page that stores the contents of this address will be
 *              put in (if required).
 *
 *  @return     void
 ****************************************************************************************/
static void vmem_put_page_into_mem(int address);

static void vmem_init(void)
{
    //TODO:
    PDEBUG("initVmemAccess");
    /* Create System V shared memory */
    key_t key = ftok(SHMKEY, SHMPROCID);
    TEST_AND_EXIT_ERRNO(key == -1, "ftok");

    /* We are only using the shm, don't set the IPC_CREAT flag */
    int shm_id = shmget(key, SHMSIZE, 0666);
    TEST_AND_EXIT_ERRNO(shm_id == -1, "shmget error");

    /* attach shared memory to vmem */

    vmem = shmat(shm_id, NULL, 0);
    TEST_AND_EXIT_ERRNO((struct vmem_struct *)-1 == vmem, "shmat error");
}

static void vmem_put_page_into_mem(int address)
{
    PDEBUG("vmem_put_page_into_mem");
    //TODO:
    /* Speicherverwaltung auffordern, Seite aus dem <pagefile> in den Hauptspeicher zu laden: */

    int page = address / VMEM_PAGESIZE;
    //    int offset = address % VMEM_PAGESIZE;
    struct msg msg;

    /* Prüfen, ob page in einem pageframe ist. */
    PDEBUG("Prüfen, ob page in einem pageframe ist");
    if (vmem->pt[page].flags == 0) //! Vllt besser PTF_PRESENT checken?
    {
        msg.cmd = CMD_PAGEFAULT; // value gibt die einzulagernde Page mit
        msg.value = page;
        msg.g_count = g_count; //modelliert die aktuelle Zeit, indem die Anzahl der Speicherzugriffe durch vmaccess gezählt wird.
        msg.ref = ref;         //Fortlaufender Ref-Counter zur Zuordnung zwischen Befehl und Antwort.

        PDEBUG("sendMsgToMmanager");
        sendMsgToMmanager(msg); //synchdataexchange: Seite aus pagefile -> mainMemory
        PDEBUG("waitForMsg");
        // waitForMsg();
        PDEBUG("msgArrived");
        ref++;
    }

    if ((g_count % TIME_WINDOW) == 0) //Wenn Zeitintervall abgelaufen ist
    {
        PDEBUG("Zeitintervall abgelaufen");
        msg.cmd = CMD_TIME_INTER_VAL; //for aging algo
        msg.g_count = g_count;
        sendMsgToMmanager(msg);
//        waitForMsg();
    }
    g_count++;
}

int vmem_read(int address)
{
    //TODO:
    /*
    - read und write berechnen aus der virtuellen Speicheradresse die Framenummer und den Offset:

    int page = (address & 0x1000) >> 3;             //page: Bit 12-15
    int offset = address & 0x0111;                  //Offset: Bit 0-11
    
    if (vmem->pt[page].frame == VOID_IDX)           //Seitenfehler: no frame reference in page table
    {
        vmem_put_page_into_mem(address);            //-> put page to frame in mainMemory
    } 
    else                                            //frame reference for page in page table
    {
        int pageFrame = vmem->pt[page].frame;       //-> get page frame from page table
    }
    */

    /*Initialize the virtual memory if accessed for the first time*/
    if (vmem == NULL)
    {
        vmem_init();
    }

    PDEBUG("vmemRead");

    int page = address / VMEM_PAGESIZE;
    int offset = address % VMEM_PAGESIZE;
    int frame = vmem->pt[page].frame;
    PDEBUG("put page to frame in mainMemory");
    vmem_put_page_into_mem(address); //put page to frame in mainMemory
    PDEBUG("read from memory");
    int value = vmem->mainMemory[frame * VMEM_PAGESIZE + offset]; //read from memory
    PDEBUG("set R-Bit");
    vmem->pt[page].flags = vmem->pt[page].flags | PTF_REF; //set R-Bit

    return value;
}

void vmem_write(int address, int data)
{
    //TODO:
    /*
    - read und write berechnen aus der virtuellen Speicheradresse die Framenummer und den Offset
    - wenn die benötigte Seite nicht geladen ist 
            -> vmem_put_page_into_mem(address); (pagefile -> mainMemory)
    */

    /*Initialize the virtual memory if accessed for the first time*/
    if (vmem == NULL)
    {
        vmem_init();
    }
    PDEBUG("vmemWrite");
    int page = address / VMEM_PAGESIZE;
    int offset = address % VMEM_PAGESIZE;
    int frame = vmem->pt[page].frame;

    PDEBUG("put page to frame in mainMemory");
    vmem_put_page_into_mem(address); //put page to frame in mainMemory

    PDEBUG("write in memory");
    vmem->mainMemory[frame * VMEM_PAGESIZE + offset] = data; //write in memory
    PDEBUG("set flags (dirty & R-Bit)");
    vmem->pt[page].flags = vmem->pt[page].flags | PTF_DIRTY | PTF_REF; //set flags (dirty & R-Bit)
}

extern void vmem_close(void)
{
    //TODO:
}

// EOF

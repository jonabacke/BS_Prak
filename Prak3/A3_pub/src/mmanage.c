/**
 * @file mmanage.c
 * @author Prof. Dr. Wolfgang Fohl, HAW Hamburg
 * @date  2014

 * @brief Memory Manager module of TI BSP A3 virtual memory
 * 
 * This is the memory manager process that
 * works together with the vmaccess process to
 * manage virtual memory management.
 *
 * mvappl sends synchronious command to memory manager via
 * module syncdataexchange.
 *
 * This process starts shared memory, so
 * it has to be started prior to the vmaccess process.
 *
 */

#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mmanage.h"
#include "debug.h"
#include "pagefile.h"
#include "logger.h"
#include "syncdataexchange.h"
#include "vmem.h"



/*
 * variables for memory management
 */

static int pf_count = 0;               //!< page fault counter
static int shm_id = -1;                //!< shared memory id. Will be used to destroy shared memory when mmanage terminates

static void (*pageRepAlgo) (int, int*, int*) = NULL; //!< selected page replacement algorithm according to parameters of mmanage

/* information used for ageing replacement strategy. For each frame, which stores a valid page, 
 * the age and and the corresponding page will be stored.
 */

struct age {
   unsigned char age;  //!< 8 bit counter for aging page replacement algorithm
   int page;           //!< page belonging to this entry
 };

struct age age[VMEM_NFRAMES];

static struct vmem_struct *vmem = NULL; //!< Reference to shared memory



int main(int argc, char **argv) {
    struct sigaction sigact;
    init_pagefile(); // init page file
    open_logger();   // open logfile

    // Setup IPC for sending commands from vmapp to mmanager
    setupSyncDataExchange();

    // Create shared memory and init vmem structure 
    vmem_init();
    TEST_AND_EXIT_ERRNO(!vmem, "Error initialising vmem");
    PRINT_DEBUG((stderr, "vmem successfully created\n"));

    // init aging info
    for(int i = 0; i < VMEM_NFRAMES; i++) {
       age[i].page = VOID_IDX;
       age[i].age = 0;
    }

    // scan parameter 
    pageRepAlgo = find_remove_fifo;
    scan_params(argc, argv);

    /* Setup signal handler */
    sigact.sa_handler = sighandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART; // damit mq_receive man eine signal neu gestartet wird

    TEST_AND_EXIT_ERRNO(sigaction(SIGUSR2, &sigact, NULL) == -1, "Error installing signal handler for USR2");
    PRINT_DEBUG((stderr,  "USR2 handler successfully installed\n"));

    TEST_AND_EXIT_ERRNO(sigaction(SIGINT, &sigact, NULL) == -1, "Error installing signal handler for INT");
    PRINT_DEBUG((stderr, "INT handler successfully installed\n"));

    // Server Loop, waiting for commands from vmapp
    while(1) {
        struct msg m = waitForMsg();
        switch(m.cmd){
	    case CMD_PAGEFAULT:
                 allocate_page(m.value, m.g_count);
            break;
        case CMD_TIME_INTER_VAL:
                if (pageRepAlgo == find_remove_aging) {
                   update_age_reset_ref();
                }
            break;
           default:
              TEST_AND_EXIT(true, (stderr, "Unexpected command received from vmapp\n"));
        }
        sendAck();
    }
    return 0;
}

void scan_params(int argc, char **argv) {
    int i = 0;
    bool param_ok = false;
    char * programName = argv[0];

    // scan all parameters (argv[0] points to program name)
    if (argc > 2) print_usage_info_and_exit("Wrong number of parameters.\n", programName);

    for (i = 1; i < argc; i++) {
        param_ok = false;
        if (0 == strcasecmp("-fifo", argv[i])) {
            // page replacement strategies fifo selected 
            pageRepAlgo = find_remove_fifo;
            param_ok = true;
        }
        if (0 == strcasecmp("-clock", argv[i])) {
            // page replacement strategies clock selected 
            pageRepAlgo = find_remove_clock;
            param_ok = true;
        }
        if (0 == strcasecmp("-aging", argv[i])) {
            // page replacement strategies aging selected 
            pageRepAlgo = find_remove_aging;
            param_ok = true;
        }
        if (!param_ok) print_usage_info_and_exit("Undefined parameter.\n", programName); // undefined parameter found
    } // for loop
}

void print_usage_info_and_exit(char *err_str, char *programName) {
    fprintf(stderr, "Wrong parameter: %s\n", err_str);
    fprintf(stderr, "Usage : %s [OPTIONS]\n", programName);
    fprintf(stderr, " -fifo     : Fifo page replacement algorithm.\n");
    fprintf(stderr, " -clock    : Clock page replacement algorithm.\n");
    fprintf(stderr, " -aging    : Aging page replacement algorithm.\n");
    fprintf(stderr, " -pagesize=[8,16,32,64] : Page size.\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
}

void sighandler(int signo) {
if(signo == SIGUSR2) {
        dump_pt();
    } else if(signo == SIGINT) {
        cleanup();
        exit(EXIT_SUCCESS);
    }  
}

void dump_pt(void) {
    int i;
    int ncols = 8;

    fprintf(stderr,
            "\n======================================\n"
            "\tPage Table Dump\n");

    fprintf(stderr, "VIRT MEM SIZE    = \t %d\n", VMEM_VIRTMEMSIZE);
    fprintf(stderr, "PHYS MEM SIZE    = \t %d\n", VMEM_PHYSMEMSIZE);
    fprintf(stderr, "PAGESIZE         = \t %d\n", VMEM_PAGESIZE);
    fprintf(stderr, "Number of Pages  = \t %d\n", VMEM_NPAGES);
    fprintf(stderr, "Number of Frames = \t %d\n", VMEM_NFRAMES);

    fprintf(stderr, "======================================\n");
    fprintf(stderr, "shm_id: \t %x\n", shm_id);
    fprintf(stderr, "pf_count: \t %d\n", pf_count);
    for(i = 0; i < VMEM_NPAGES; i++) {
        fprintf(stderr,
                "Page %5d, Flags %x, Frame %10d, age 0x%2X,  \n", i,
                vmem->pt[i].flags, vmem->pt[i].frame, age[vmem->pt[i].frame].age);
    }
    fprintf(stderr,
            "\n\n======================================\n"
            "\tData Dump\n");
    for(i = 0; i < (VMEM_NFRAMES * VMEM_PAGESIZE); i++) {
        fprintf(stderr, "%10x", vmem->mainMemory[i]);
        if(i % ncols == (ncols - 1)) {
            fprintf(stderr, "\n");
        }
        else {
            fprintf(stderr, "\t");
        }
    }
}

/* Your code goes here... */

void cleanup(void) {
    //TODO:
    /*
    - im Signalhandler zu SIGINT einhängen
    */
}

void vmem_init(void) {
    //TODO:

    
    /* Create System V shared memory */
    key_t key = ftok(SHMKEY, SHMPROCID);
    
    /* We are creating the shm, so set the IPC_CREAT flag */
    int shm_id = shmget(key, SHMSIZE, IPC_CREAT | 0666);
    TEST_AND_EXIT_ERRNO(shm_id, "shmget error");

    for (int i = 0; i < VMEM_NPAGES; i++) {
        vmem->pt[i].flags = 0;                  //flags auf '0' setzen
        vmem->pt[i].frame = VOID_IDX;           //frame nicht zugeteilt (-1)
    }

    /* Attach shared memory to vmem (virtual memory) */

    int shm = shmat(shm_id, NULL, 0);
    TEST_AND_EXIT_ERRNO(shm, "shmat error");

    /* Fill with zeros */
    memset(vmem, 0, SHMSIZE);
}

int find_unused_frame() {
    
    /* freien Seitenrahmen suchen */
    for (int i = 0; i < VMEM_NPAGES; i = i + 8) {
        if (vmem->pt[i].flags == 0) 
        {
            return i;
        }
        else continue;
    }
    /* kein freier pageframe gefunden*/
    return VOID_IDX; 
}

void allocate_page(const int req_page, const int g_count) {

    /* Log action */
    le.req_pageno = req_page;
    le.replaced_page = removedPage;
    le.alloc_frame = frame;
    le.g_count = g_count; 
    le.pf_count = pf_count;         //log page fault
    logger(le);
    //TODO: call replacement algo from here? //update page table
    
}

void fetchPage(int page, int frame){
    //TODO:
    /*
    Prozess <mmanage> nimmt eine Seite aus Datei <pagefile>
    -> lagert diese in einen Seitenrahmen von mainMemory ein
    -> Wenn dabei eine Seite aus einem Seitenrahmen verdrängt wird, schreibt <mmanage> 
        die modifizierte Seite an die richtige Stelle in der Datei <pagefile> zurück
    */
}

void removePage(int page) {
    //TODO:
    //removedPage = page;
}

void find_remove_fifo(int page, int * removedPage, int *frame){
    //TODO:
    /*
    -bei Seitenfehler: 
            logger();
    FIFO
    - Seiten stehen als verkettete Liste im Speicher
    - Bei Seitenfehler:
        - Element/Seite mit ältestem Ladezeitpunkt (Listenkopf) wird entfernt
        - Neues Element am Ende der Liste einfügen
        -> R-Bit wird nicht benötigt
        -> keine Unterscheidung zwischen viel und wenig genutzten Seiten
            (älteste Seite wird ausgelagert)
    */
}

static void find_remove_aging(int page, int * removedPage, int *frame){
    //TODO:
    /*
    -bei Seitenfehler: 
            logger();
    AGING:
    - Jede Seite hat einen SW-Zähler, der mit 0 initialisiert ist.
    - zyklisch (alle 20 ms):
        - Zähler um 1 nach rechts shiften (/2)
        - R-Bit auf MSB (links)
        - R-Bit zurücksetzen
        - bei 8-Bit Breite von Age: age beim Einlagern der Seite auf 0x80 setzen
    */
}

static void update_age_reset_ref(void) {
    //TODO:
} 

static void find_remove_clock(int page, int * removedPage, int *frame){
    //TODO:
    /*
    -bei Seitenfehler: 
            logger();
    CLOCK
    - 2cFIFO, aber zyklische Liste:
        ->  Pointer auf Kopfelement

            checkElement(pointer) {
                if (R == 0) {
                    Seite löschen bzw. durch neues Element ersetzen; 
                    R = 1;
                    Pointer++;
                }
                else if (R == 1) {
                    R = 0;
                    Pointer++;
                    checkElement(pointer);
                }
            }
    */
}

// EOF


/*
- Virtueller Speicher wird in pagefile nachgebildet
- Größe des virtuellen Speichers: VMEM_VIRTMEMSIZE Byte
    -> in Seiten der Größe VMEM_PAGESIZE Byte unterteilt
    -> werden in der Datei <pagefile> hintereinander abgelegt
*/
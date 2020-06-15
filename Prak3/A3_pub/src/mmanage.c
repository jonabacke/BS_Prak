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

#include "mmanage.h"

/*
 * variables for memory management
 */

static int pf_count = 0; //!< page fault counter
static int shm_id = -1;  //!< shared memory id. Will be used to destroy shared memory when mmanage terminates

static void (*pageRepAlgo)(int, int *, int *) = NULL; //!< selected page replacement algorithm according to parameters of mmanage

/* information used for ageing replacement strategy. For each frame, which stores a valid page, 
 * the age and and the corresponding page will be stored.
 */

struct age
{
    unsigned char age; //!< 8 bit counter for aging page replacement algorithm
    int page;          //!< page belonging to this entry
};

struct age age[VMEM_NFRAMES];

static struct vmem_struct *vmem = NULL; //!< Reference to shared memory

int main(int argc, char **argv)
{
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
    for (int i = 0; i < VMEM_NFRAMES; i++)
    {
        age[i].page = VOID_IDX;
        age[i].age = 0;
    }

    // scan parameter
    pageRepAlgo = find_remove_fifo; //no param set -> fifo
    scan_params(argc, argv);        //else page replacing algo = param

    /* Setup signal handler */
    sigact.sa_handler = sighandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART; // damit mq_receive man eine signal neu gestartet wird

    TEST_AND_EXIT_ERRNO(sigaction(SIGUSR2, &sigact, NULL) == -1, "Error installing signal handler for USR2");
    PRINT_DEBUG((stderr, "USR2 handler successfully installed\n"));

    TEST_AND_EXIT_ERRNO(sigaction(SIGINT, &sigact, NULL) == -1, "Error installing signal handler for INT");
    PRINT_DEBUG((stderr, "INT handler successfully installed\n"));

    // Server Loop, waiting for commands from vmapp
    while (1)
    {
        struct msg m = waitForMsg();
        switch (m.cmd)
        {
        case CMD_PAGEFAULT:
            allocate_page(m.value, m.g_count);
            break;
        case CMD_TIME_INTER_VAL:
            if (pageRepAlgo == find_remove_aging)
            {
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

void scan_params(int argc, char **argv)
{
    int i = 0;
    bool param_ok = false;
    char *programName = argv[0];

    // scan all parameters (argv[0] points to program name)
    if (argc > 2)
        print_usage_info_and_exit("Wrong number of parameters.\n", programName);

    for (i = 1; i < argc; i++)
    {
        param_ok = false;
        if (0 == strcasecmp("-fifo", argv[i]))
        {
            // page replacement strategies fifo selected
            pageRepAlgo = find_remove_fifo;

            fifoPointer = 0;

            param_ok = true;
        }
        if (0 == strcasecmp("-clock", argv[i]))
        {
            // page replacement strategies clock selected
            pageRepAlgo = find_remove_clock;

            clockPointer = 0;

            param_ok = true;
        }
        if (0 == strcasecmp("-aging", argv[i]))
        {
            // page replacement strategies aging selected
            pageRepAlgo = find_remove_aging;
            param_ok = true;
        }
        if (!param_ok)
            print_usage_info_and_exit("Undefined parameter.\n", programName); // undefined parameter found
    }                                                                         // for loop
}

void print_usage_info_and_exit(char *err_str, char *programName)
{
    fprintf(stderr, "Wrong parameter: %s\n", err_str);
    fprintf(stderr, "Usage : %s [OPTIONS]\n", programName);
    fprintf(stderr, " -fifo     : Fifo page replacement algorithm.\n");
    fprintf(stderr, " -clock    : Clock page replacement algorithm.\n");
    fprintf(stderr, " -aging    : Aging page replacement algorithm.\n");
    fprintf(stderr, " -pagesize=[8,16,32,64] : Page size.\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
}

void sighandler(int signo)
{
    if (signo == SIGUSR2)
    {
        dump_pt();
    }
    else if (signo == SIGINT)
    {
        cleanup();
        exit(EXIT_SUCCESS);
    }
}

void dump_pt(void)
{
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
    for (i = 0; i < VMEM_NPAGES; i++)
    {
        fprintf(stderr,
                "Page %5d, Flags %x, Frame %10d, age 0x%2X,  \n", i,
                vmem->pt[i].flags, vmem->pt[i].frame, age[vmem->pt[i].frame].age);
    }
    fprintf(stderr,
            "\n\n======================================\n"
            "\tData Dump\n");
    for (i = 0; i < (VMEM_NFRAMES * VMEM_PAGESIZE); i++)
    {
        fprintf(stderr, "%10x", vmem->mainMemory[i]);
        if (i % ncols == (ncols - 1))
        {
            fprintf(stderr, "\n");
        }
        else
        {
            fprintf(stderr, "\t");
        }
    }
}

/* Your code goes here... */

void cleanup(void)
{
    destroySyncDataExchange();
    // distroy shared memory
    TEST_AND_EXIT_ERRNO(-1 == shmctl(shm_id, IPC_RMID, NULL), "mmanage: shmctl failed"); // Mark vmem for deletion
    TEST_AND_EXIT_ERRNO(-1 == shmdt(vmem), "mmanage: shmdt failed");                     // detach shared memory
    PRINT_DEBUG((stderr, "\nmmanage: Shared memory successfully detached\n"));
}

void vmem_init(void)
{
    PDEBUG("vmemInitMmanage");
    /* Create System V shared memory */
    key_t key = ftok(SHMKEY, SHMPROCID);
    TEST_AND_EXIT_ERRNO(key == -1, "shmget error");

    /* We are creating the shm, so set the IPC_CREAT flag */
    int shm_id = shmget(key, SHMSIZE, IPC_CREAT | 0666);

    printf("%d", shm_id);
    TEST_AND_EXIT_ERRNO(shm_id == -1, "shmget error");

    /* Attach shared memory to vmem (virtual memory) */
    vmem = (struct vmem_struct *)shmat(shm_id, NULL, 0);

    TEST_AND_EXIT_ERRNO(vmem == (struct vmem_struct *)-1, "shmat error");

    // for (int i = 0; i < VMEM_NPAGES; i++) //! evtl weglassen?!
    // {
    //     vmem->pt[i].flags = 0;        //flags auf '0' setzen
    //     vmem->pt[i].frame = VOID_IDX; //frame nicht zugeteilt (-1)
    // }

    /* Fill with zeros */
    memset(vmem, 0, SHMSIZE);
}

int find_unused_frame()
{

    PDEBUG("freien Seitenrahmen suchen");
    /* freien Seitenrahmen suchen */
    static int frameCounter = 0;

    int unusedFrame = VOID_IDX;

    if (frameCounter < VMEM_NFRAMES)
    {
        unusedFrame = frameCounter;
        frameCounter++;
    }

    /* kein freier pageframe gefunden*/
    return unusedFrame;
}

void allocate_page(const int req_page, const int g_count)
{
    PDEBUG("allocate_page");
    pf_count++;

    PRINT_DEBUG((stderr, "allocate Page called reqPage: %d\n", req_page));
    int removedPage = VOID_IDX;
    int frame = find_unused_frame(); //finde freien page frame...

    if (frame == VOID_IDX) //...unused frame found:
    {
        (*pageRepAlgo)(req_page, &removedPage, &frame);
        PRINT_DEBUG((stderr, "Page Rep Algorithm determined to remove page [%d], and page [%d] int frame [%d]\n", removedPage, req_page, frame));
        removePage(removedPage);
    }
    fetchPage(req_page, frame);
    // else //...no unused page frames left:
    // {
    //     findPageToRemove(removedPage);
    //     frame = vmem->pt[*removedPage].frame;
    //     /* call page replacement algo to place a page without removing a page */
    //     if (pageRepAlgo == find_remove_fifo)
    //         find_remove_fifo(req_page, NULL, &frame);
    //     else if (pageRepAlgo == find_remove_clock)
    //         find_remove_clock(req_page, NULL, &frame);
    //     else /*(pageRepAlgo == find_remove_aging)*/
    //         find_remove_aging(req_page, NULL, &frame);

    //     //TODO: call replacement algo from here? //update page table
    // }
    struct logevent le;
    PDEBUG("logData");
    /* Log action */
    le.req_pageno = req_page;
    le.replaced_page = removedPage;
    le.alloc_frame = frame;
    le.g_count = g_count;
    le.pf_count = pf_count; //log page fault
    logger(le);

    //!ack?
    //    sendAck();
}

// //!TODO: implementieren..
// void findPageToRemove(int *pageToRemove)
// {

//     PDEBUG("findPageToRemove");
//     if (pageRepAlgo == find_remove_fifo)
//     {
//         // pageToRemove = fifo->firstElement.page;   //first element of fifo to be removed

//         fifoPointer = (fifoPointer + 1) % VMEM_NFRAMES;
//         *pageToRemove = fifoPointer;
//     }
//     else if (pageRepAlgo == find_remove_clock)
//     {
//         //        while ((clock[clockPointer].flags & PTF_REF) == PTF_REF)
//         //        {
//         //            clock[clockPointer].flags = clock[clockPointer].flags & ~PTF_REF;
//         //            clockPointer = (clockPointer + 1) % VMEM_NFRAMES;
//         //            *pageToRemove = clockPointer;
//         //        }
//         *pageToRemove = clockPointer;
//     }
//     else if (pageRepAlgo == find_remove_aging)
//     {
//         *pageToRemove = 0;
//         for (int i = 0; i < VMEM_NPAGES; i++)
//         {
//             if (aging[i].swCounter < aging[*pageToRemove].swCounter)
//             {
//                 *pageToRemove = i;
//             }
//         }
//     }
// }

void fetchPage(int page, int frame)
{
    PRINT_DEBUG((stderr, "fetchPage called. Page: %d, Frame: %d\n", page, frame));
    int newFrameContent[VMEM_PAGESIZE] = {0};
    /* fetch page from pagefile and write it to virtual memory*/
    fetch_page_from_pagefile(page, newFrameContent);
    PRINT_DEBUG((stderr, "fetchedPage. Page: %d from disk\n", page));

    checkAddrSpace(frame, VMEM_NFRAMES);
    checkAddrSpace(page, VMEM_NPAGES);

    for (int i = 0; i < VMEM_PAGESIZE; i++)
    {
        vmem->mainMemory[(frame * VMEM_PAGESIZE) + i] = newFrameContent[i]; // Reinladen des Contents
    }

    /* Update page table */
    vmem->pt[page].frame = frame;       //set page frame reference
    vmem->pt[page].flags = PTF_PRESENT; //set Present-Bit and R-Bit

    //TODO: fehlt noch etwas? muss R-Bit gesetzt werden?
}

void removePage(int page)
{
    PDEBUG("removePage");
    checkAddrSpace(page, VMEM_NPAGES);
    /* if page has been changed */
    if ((vmem->pt[page].flags & PTF_DIRTY) == PTF_DIRTY) //check dirty flag
    {
        int value = vmem->pt[page].frame; //read changed page from memory
        int *pagePointer = &(vmem->mainMemory[value * VMEM_PAGESIZE]);
        store_page_to_pagefile(page, pagePointer); //save changed page in pagefile
    }

    /* Update page table */
    vmem->pt[page].frame = VOID_IDX; //remove page frame reference
    vmem->pt[page].flags = 0;        //vmem->pt[page].flags | !PTF_PRESENT | !PTF_DIRTY | !PTF_REF; //reset flags

    //TODO: überlegen, ob das als 'löschen' so ausreicht.. denke das reicht
}

static void init_fifo(int *fifo)
{
    static bool init = true;
    if (init)
    {
        init = false;
        // Alle eingelagerten Seiten in die Queue aufnehmen.
        for (int i = 0; i < VMEM_NPAGES; i++)
        {
            int currframe = vmem->pt[i].frame;
            if (currframe != VOID_IDX)
            { // Wenn die Seite entweder Dirty oder Present ist
                checkAddrSpace(currframe, VMEM_NFRAMES);
                fifo[currframe] = i; // Wird die seite (i) an die frame stelle der Queue geschrieben
            }
        }
    }
}

void find_remove_fifo(int page, int *removedPage, int *frame)
{
    PDEBUG("find_remove_fifo");
    init_fifo(fifo);

    checkAddrSpace(page, VMEM_NPAGES);
    // if (removedPage != NULL) //found a page to remove
    // {
    //     /* remove page to free page frame */
    //     removePage(*removedPage);

    //     /* remove page element from the head of the fifo list */
    //     //Element wird ueberschrieben und pointer neu gesetzt.
    // }

    /* fetch page from pagefile */
    // fetchPage(page, *frame);

    if (fifo[fifoPointer] != VOID_IDX)
    {
        *removedPage = fifo[fifoPointer];
    }

    /* insert new page element at the end of the fifo list */
    fifo[fifoPointer] = page;
    *frame = fifoPointer;
    fifoPointer++;
    fifoPointer = fifoPointer % VMEM_NFRAMES;
}

static void initAging()
{
    static bool isInit = false;
    if (!isInit)
    {
        isInit = true;
        for (int i = 0; i < VMEM_NPAGES; i++)
        {
            int currFlag = vmem->pt[i].flags;
            if (currFlag != 0)
            {
                int frameNum = vmem->pt[i].frame;
                checkAddrSpace(frameNum, VMEM_NFRAMES);
                age[frameNum].page = i; // Wird die seite (i) an die frame stelle der Queue geschrieben
                                        //age[frameNum].refBit = false;
            }
        }
        update_age_reset_ref();
    }
}

static void find_remove_aging(int page, int *removedPage, int *frame)
{
    PDEBUG("find_remove_aging");
    initAging();
    checkAddrSpace(page, VMEM_NPAGES);
    // if (removedPage != NULL) //found a page to remove
    // {
    //     /* remove page to free page frame */
    //     removePage(*removedPage);
    // }

    /* fetch page from pagefile */
    // fetchPage(page, *frame);

    if (vmem->pt[page].frame == VOID_IDX)
    {
        int oldestFrame = 0;

        for (int i = 0; i < VMEM_NFRAMES; i++)
        {
            if (age[i].age <= age[oldestFrame].age)
            {
                oldestFrame = i;
            }
        }

        // set Ouput Parameters
        *removedPage = age[oldestFrame].page;
        *frame = oldestFrame;

        // Insert new Page
        age[oldestFrame].page = page;
        age[oldestFrame].age = 1 << (AGE_REGISTER_SIZE - 1);
    }

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

static void update_age_reset_ref(void)
{
    PDEBUG("update_age_reset_ref");
    //TODO:
    for (int i = 0; i < VMEM_NPAGES; i++)
    {
        u_int8_t mask = 0;
        if ((vmem->pt[i].flags & PTF_PRESENT) == PTF_PRESENT)
        {
            if (((vmem->pt[i].flags & PTF_REF) == PTF_REF))
            {
                vmem->pt[i].flags = vmem->pt[i].flags & (~PTF_REF); // Reset des Ref bits
                mask = (1 >> (AGE_REGISTER_SIZE - 1));
            }
            int currFrame = vmem->pt[i].frame;
            age[currFrame].age = (age[currFrame].age >> 1) | mask;
            PRINT_DEBUG((stderr, "updated age from frame [%d]. New Age: [%u] Mask:[%u]\n", i, (unsigned int)age[i].age, mask));
        }
    }
}

static void initClk(int *clock)
{
    static bool init = true;
    if (init)
    {
        init = false;
        // Alle eingelagerten Seiten in die Queue aufnehmen.
        for (int i = 0; i < VMEM_NPAGES; i++)
        {
            int currFlag = vmem->pt[i].flags;
            if (currFlag != 0)
            { // Wenn die Seite entweder Dirty oder Present ist
                int frameNum = vmem->pt[i].frame;
                checkAddrSpace(frameNum, VMEM_NFRAMES);
                clock[frameNum] = i; // Wird die seite (i) an die frame stelle der Queue geschrieben
            }
        }
    }
}

static void find_remove_clock(int page, int *removedPage, int *frame)
{
    PDEBUG("find_remove_clock");
    initClk(clock);
    // if (removedPage != NULL) //found a page to remove
    // {
    //     /* remove page to free page frame */
    //     removePage(*removedPage);
    // }

    // /* fetch page from pagefile */
    // fetchPage(page, *frame);

    char currentFlag = vmem->pt[clock[clockPointer]].flags;

    while ((currentFlag & PTF_REF) == PTF_REF)
    {
        vmem->pt[clock[clockPointer]].flags = currentFlag & ~PTF_REF;
        clockPointer++;
        clockPointer = clockPointer % VMEM_NFRAMES;
        currentFlag = vmem->pt[clock[clockPointer]].flags;
    }

    int currentPage = clock[clockPointer];
    if (currentPage != VOID_IDX)
    {
        *removedPage = currentPage;
    }
    clock[clockPointer] = page;

    *frame = clockPointer;
    clockPointer++;
    clockPointer = clockPointer % VMEM_NFRAMES;

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

static FILE *logfile = NULL; //!< Reference to logfile

void open_logger(void)
{
    /* Open logfile */
    logfile = fopen(MMANAGE_LOGFNAME, "w");
    TEST_AND_EXIT_ERRNO(!logfile, "Error creating logfile");
}

void close_logger(void)
{
    fclose(logfile);
}

/* Do not change!  */
void logger(struct logevent le)
{
    fprintf(logfile, "Page fault %10d, Global count %10d:\n"
                     "Removed: %10d, Allocated: %10d, Frame: %10d\n",
            le.pf_count, le.g_count,
            le.replaced_page, le.req_pageno, le.alloc_frame);
    fflush(logfile);
}
// EOF

/*
- Virtueller Speicher wird in pagefile nachgebildet
- Größe des virtuellen Speichers: VMEM_VIRTMEMSIZE Byte
    -> in Seiten der Größe VMEM_PAGESIZE Byte unterteilt
    -> werden in der Datei <pagefile> hintereinander abgelegt
*/

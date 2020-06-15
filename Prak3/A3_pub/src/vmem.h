/**
 * @file vmem.h
 * @author Wolfgang Fohl, HAW Hamburg 
 * @brief Global Definitions for TI BS A3 - model of virtual memory management

 * First Version : Wolfgang Fohl HAW Hamburg
 * Dec 2015 : Delete BitMap for free frames (Franz Korf, HAW Hamburg)
 * Dec 2015 : Set memory algorithm via command line parameter 
 * Dec 2015 : Set define for PAGESIZE and VMEM_ALGO via compiler -D option (Franz Korf, HAW Hamburg)
 * Dec 2015 : Add some documentation (Franz Korf, HAW Hamburg)
 * April 2018 : New IPC for mmanage and vmappl (Franz Korf, HAW Hamburg)
 */


#ifndef _VMEM_H
#define _VMEM_H


#define SHMKEY          "./src/vmem.h" //!< First paremater for shared memory generation via ftok function
#define SHMPROCID       1234           //!< Second paremater for shared memory generation via ftok function


/**
 * Constant VMEM_PAGESIZE will be set via compiler -D option. 
 * Default value : 8
 * value range : 8 16 32 64 
 */
#ifndef VMEM_PAGESIZE
#define VMEM_PAGESIZE 8
#endif


/* Sizes */
#define VMEM_VIRTMEMSIZE 1024   				//!< Size of virtual address space of the process
#define VMEM_PHYSMEMSIZE  128   				//!< Size of physical memory
#define VMEM_NPAGES     (VMEM_VIRTMEMSIZE / VMEM_PAGESIZE)	//!< Total number of pages                 //(1024 Byte/ 8 Byte = 128 Pages)
#define VMEM_NFRAMES (VMEM_PHYSMEMSIZE / VMEM_PAGESIZE)		//!< Total number of (page) frames/        //(128 Byte/ 8 Byte = 16 Pageframes) 


/**
 * page table flags used by this simulation
 */
#define PTF_PRESENT     1
#define PTF_DIRTY       2 //!< store: need to write 
#define PTF_REF         4       

#define VOID_IDX -1       //!< Constant for invalid page or frame reference 
#define DEBUG 
#ifdef DEBUG
#define PDEBUG(x)                    \
    do {                             \
        printf(x); \
        printf("\n"); \
    } while (0)
#else
#define PDEBUG(x) \
    do {          \
    } while (0)
#endif

/**
 * Page table entry
 */
struct pt_entry {
   int flags;             //!< See definition of PTF_* flags 
   int frame;             //!< Frame idx; frame == VOID_IDX: unvalid reference  
};


/**
 * The data structure stored in shared memory
 */
struct vmem_struct {
    struct pt_entry pt[VMEM_NPAGES];               //!< page table                                     //Seitentabelle (128 Eintraege)
    int mainMemory[VMEM_NFRAMES * VMEM_PAGESIZE];  //!< main memory used by virtual memory simulation  //(16 Pageframes * 8 Byte = 128 Byte (PhysMemSize/repräs. Hauptspeicher))
};


#define SHMSIZE (sizeof(struct vmem_struct)) //!< size of virtual memory 


#endif /* _VMEM_H */

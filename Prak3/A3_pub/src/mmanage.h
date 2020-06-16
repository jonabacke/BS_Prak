/**
 * @file mmanage.h
 * @author Prof. Dr. Wolfgang Fohl, HAW Hamburg
 * @date  2013
 * @brief Header file of virtual memory management module.
 */

#ifndef _MMANAGE_H
#define _MMANAGE_H

#include "vmem.h"
#include <semaphore.h>
#include <stdint.h>

#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "vmaccess.h"
#include "debug.h"
#include "pagefile.h"
// #include "logger.h"
#include "syncdataexchange.h"

/** 
 * Event struct for logging 
 */
struct logevent
{
    int req_pageno;    //!< requested page number
    int replaced_page; //!< replaced page number
    int alloc_frame;   //!< selected frame
    int pf_count;      //!< current number of page faults
    int g_count;       //!< gobal quasi time stamp
} le;

#define MMANAGE_LOGFNAME "./logfile.txt" //!< logfile name
#define AGE_REGISTER_SIZE 8



typedef struct aging_struct
{
    struct pt_entry aging;
    uint8_t swCounter;
} Aging;




int clock[VMEM_NFRAMES];
int clockPointer = 0;

Aging aging[VMEM_NFRAMES];

/*
 * Signatures of private / static functions
 */

/**
 *****************************************************************************************
 *  @brief      This function fetchs a page from disk into memory. The page table 
 *              will be updated.
 *
 *  @param      page Number of the page that should be fetched
 *  @param      frame Number of frame that should contain the page.
 * 
 *  @return     void 
 ****************************************************************************************/
static void fetchPage(int page, int frame);

/**
 *****************************************************************************************
 *  @brief      This function removes a page from main memory. If the page was modified,
 *              it will be written back to disk. The page table will be updated.
 *
 *  @param      page Number of the page that should be removed
 * 
 *  @return     void 
 ****************************************************************************************/
static void removePage(int page);

/**
 *****************************************************************************************
 *  @brief      This function initializes the virtual memory.
 *              In particular it creates the shared memory. The application just attachs 
 *              to the shared memory.
 *
 *  @return     void 
 ****************************************************************************************/
static void vmem_init(void);

/**
 *****************************************************************************************
 *  @brief      This function finds an unused frame. At the beginning all frames are 
 *              unused. A frame will never change it's state form used to unused.
 *
 *              Since the log files to be compared with contain the allocated frames, unused 
 *              frames must always be assigned the same way. Here, the frames are assigned 
 *              according to ascending frame number.
 *            
 *  @return     idx of the unused frame with the smallest idx. 
 *              If all frames are in use, VOID_IDX will be returned.
 ****************************************************************************************/
static int find_unused_frame();

/**
 *****************************************************************************************
 *  @brief      This function will be called when a page fault has occurred. It allocates 
 *              a new page into memory. If all frames are in use the corresponding page 
 *              replacement algorithm will be called.
 *              Please take into account that allocate_page must update the page table 
 *              and log the page fault as well.
 *
 *  @param      req_page  The page that must be allocated due to the page fault. 

 *  @param      g_count   Current g_count value
 *
 *  @return     void 
 ****************************************************************************************/
static void allocate_page(const int req_page, const int g_count);

/**
 *****************************************************************************************
 *  @brief      This function is the signal handler attached to system call sigaction
 *              for signal SIGUSR2 and SIGINT.
 *              These signals have the same signal handler. Based on the parameter 
 *              signo the corresponding action will be started.
 *
 *  @param      signo Current signal that has be be handled.
 * 
 *  @return     void 
 ****************************************************************************************/
static void sighandler(int signo);

/**
 *****************************************************************************************
 *  @brief      This function dumps the page table to stderr.
 *
 *  @return     void 
 ****************************************************************************************/
static void dump_pt(void);

/**
 *****************************************************************************************
 *  @brief      This function implements page replacement algorithm aging.
 *
 *  @param      page Number of page that should be loaded into memory.
 *
 *  @param      removedPage Number of page that has been selected for replacement.
 *              If an unused frame has selected, this parameter will not be 
 *              modified.
 *
 *  @param      frame Number of frame that will be used to store the page.
 *
 ****************************************************************************************/
static void find_remove_aging(int page, int *removedPage, int *frame);

/**
 *****************************************************************************************
 *  @brief      This function does aging for aging page replacement algorithm.
 *              It will be called periodic based on g_count.
 *              This function must be used only when aging algorithm is activ.
 *              Otherwise update_age_reset_ref may interfere with other page replacement 
 *              alogrithms that base on PTF_REF bit.
 *
 *  @return     void
 ****************************************************************************************/
static void update_age_reset_ref(void);

/**
 *****************************************************************************************
 *  @brief      This function implements page replacement algorithm fifo.
 *
 *  @param      page Number of page that should be loaded into memory.
 *
 *  @param      removedPage Number of page that has been selected for replacement.
 *              If an unused frame has selected, this parameter will not be 
 *              modified.
 *
 *  @param      frame Number of frame that will be used to store the page.
 *
 ****************************************************************************************/
static void find_remove_fifo(int page, int *removedPage, int *frame);

/**
 *****************************************************************************************
 *  @brief      This function implements page replacement algorithm clock.
 *
 *  @param      page Number of page that should be loaded into memory.
 *
 *  @param      removedPage Number of page that has been selected for replacement.
 *              If an unused frame has selected, this parameter will not be 
 *              modified.
 *
 *  @param      frame Number of frame that will be used to store the page.
 *
 ****************************************************************************************/
static void find_remove_clock(int page, int *removedPage, int *frame);

/**
 *****************************************************************************************
 * @brief   Find a page to remove from the page frame depending on the chosen page 
 *          replacement algorithm in case of a page fault:
 *          find_remove_fifo --> 'eldest' page will be removed
 *          find_remove_clock --> 'eldest' page with R-Bit == 0 will be removed
 *          find_remove_aging --> actually least referenced page will be removed
 * 
 * @param pageToRemove  Pointer to the page to remove from the page frame
 * 
 ****************************************************************************************/
void findPageToRemove(int *pageToRemove);

/**
 *****************************************************************************************
 *  @brief      This function cleans up when mmange runs out.
 *
 *  @return     void 
 ****************************************************************************************/
static void cleanup(void);

/**
 *****************************************************************************************
 *  @brief      This function scans all parameters of the porgram.
 *              The corresponding global variables page_rep_algo will be set.
 * 
 *  @param      argc number of parameter 
 *
 *  @param      argv parameter list 
 *
 *  @return     void 
 ****************************************************************************************/
static void scan_params(int argc, char **argv);

/**
 *****************************************************************************************
 *  @brief      This function prints an error message and the usage information of 
 *              this program.
 *
 *  @param      err_str pointer to the error string that should be printed.
 *  @param      programName pointer to the name of the the program
 *
 *  @return     void 
 ****************************************************************************************/
static void print_usage_info_and_exit(char *err_str, char *programName);


/**
 *****************************************************************************************
 *  @brief      This function creates a new logfile
 *
 *  @return     void 
 ****************************************************************************************/
void open_logger(void);

/**
 *****************************************************************************************
 *  @brief      This function closes the current logfile
 *
 *  @return     void 
 ****************************************************************************************/
void close_logger(void);

/**
 *****************************************************************************************
 *  @brief      This function writes a log entity to the logfile.
 *
 *  @param      le This stucture describes the entity that should be logged.
 *
 *  @return     void 
 ****************************************************************************************/
void logger(struct logevent le);

#endif /* _MMANAGE_H */

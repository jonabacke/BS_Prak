/**
 * @file vmaccess.h
 * @author Prof. Dr. Wolfgang Fohl, HAW Hamburg
 * @date 2010
 * @brief This module defines function to read from and write to
 * virtual memory.
 */


#ifndef _VMACCESS_H
#define _VMACCESS_H


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



/**
 *****************************************************************************************
 *  @brief      This function reads an integer value from virtual memory.
 *              If this functions access virtual memory for the first time, the 
 *              virtual memory will be setup and initialized.
 *
 *  @param      address The virtual memory address the integer value should be read from.
 * 
 *  @return     The int value read from virtual memory.
 ****************************************************************************************/
int vmem_read(int address);



/**
 *****************************************************************************************
 *  @brief      This function writes an integer value from virtual memory.
 *              If this functions access virtual memory for the first time, the 
 *              virtual memory will be setup and initialized.
 *
 *  @param      address The virtual memory address the integer value should be written to.
 *
 *  @param      data The integer value that should be written to virtual memory.
 * 
 *  @return     void
 ****************************************************************************************/
void vmem_write(int address, int data);



extern void vmem_close(void);


#endif /* _VMACCESS_H*/

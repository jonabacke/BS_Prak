/**
 * @file vmappl.h
 * @author Wolfgang Fohl, HAW Hamburg 
 * @brief  This module contains the application that will be executed on the simulated virtual memory.
 * QuickSort will sort in array of int values. This array is located in the simulated virtual memory.
 * @date 2013
 */


#ifndef _VMAPPL_H
#define _VMAPPL_H


#ifndef SEED
#define SEED   2806    //!< Default value for setup of random number generator to initialized the array to be sorted
#endif

#define LENGTH 550     //!< Length of array to be sorted
#define RNDMOD 1000    //!< Second argument of modulo operator to shrink random numbers

#define NDISPLAYCOLS 8 //!< Number of values printed in one line

#define INIT_TYPE_SEED 0   // init array with random numbers
#define INIT_TYPE_UP   1   // init array with increasing numbers
#define INIT_TYPE_DOWN 2   // init array with decreasing numbers

#define QUICK_SORT     10  // use quick sort 
#define BUBBLE_SORT    11  // use bubble  sort 


/* 
 * Signatures of private (static) functions of this module.
 */


/**
 *****************************************************************************************
 *  @brief      Initialisation of the array to be sorted 
 *
 * It will be called once during start phase.
 * The access the virtual memory via shared memory must be initialized by the
 * first call of the corresponding access function (vmem_read or vmem_write).
 * 
 *  @param      length length of the array to be initialized 
 *
 *  @return     void 
 ****************************************************************************************/
 static void init_data(int length);



/**
 *****************************************************************************************
 *  @brief      Recursive version of QuickSort 
 *
 *  @param      l address of the left-most array element to be sorted
 * 
 *  @param      r address of the right-most array element to be sorted
 *
 *  @return     void 
 ****************************************************************************************/
static void quicksort(int l, int r);



/**
 *****************************************************************************************
 *  @brief      Bubble sort 
 *
 *  @param      l address of the left-most array element to be sorted
 * 
 *  @param      r address of the right-most array element to be sorted
 *
 *  @return     void 
 ****************************************************************************************/
static void bubblesort(int l, int r);



/**
 *****************************************************************************************
 *  @brief      This function sorts the array stored in the virtual memory.
 *
 *  @param      length length of the array to be sorted 
 *
 *  @return     void 
 ****************************************************************************************/
static void sort(int length);



/**
 *****************************************************************************************
 *  @brief      This function swaps two int values of virtual memory 
 *
 *  Call this function to swap two int elements stored at addr1 and addr2 
 *  of virtual memory.
 * 
 *  @param      addr1 address of first elment in virtual memory 
 *
 *  @param      addr2 address of second elment in virtual memory 
 *
 *  @return     void 
 ****************************************************************************************/
static void swap(int addr1, int addr2);



/**
 *****************************************************************************************
 *  @brief      This function scans all parameters of the program.
 *              The corresponding global variables seed and sort_algo will be set.
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
 *  @brief      This function prints the array stored in virtual memory 
 *
 *  It prints the first "length" integer elements of virtual memory.
 *
 *  @param      length length of the array to be initialized 
 *
 *  @return     void 
 ****************************************************************************************/
static void display_data(int length);



/**
 *****************************************************************************************
 *  @brief      This function prints an error message and the usage information of 
 *              this program.
 *
 *  @param      err_str pointer to the error string that should be printed.
 *
 *  @return     void 
 ****************************************************************************************/
static void print_usage_info_and_exit(char *err_str);


#endif /*_VMAPPL_H*/

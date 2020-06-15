 /**
 * @file vmappl.c
 * @author Wolfgang Fohl, HAW Hamburg 
 * @brief  Demo application for virtual memory management model
 * @date 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vmaccess.h"
#include "my_rand.h"
#include "vmappl.h"


/*
 * static global variables
 */
static char *program_name = NULL;
static int sort_algo      = QUICK_SORT; // select default sort algorithm
static int seed           = SEED; // select default init value for random number generator 


/* 
 * functions of the module 
 */


void scan_params(int argc, char **argv) {
    int i = 0;
    bool sort_algo_param_found = false;
    bool seed_param_found      = false;
    bool param_ok              = false;
    const char *seed_str = "-seed=";

    // scan all parameters (argv[0] points to program name)
    for (i = 1; i < argc; i++) {
        param_ok = false;
        if (0 == strcasecmp("-bubblesort", argv[i])) {
            // bubblesort selected
            if (sort_algo_param_found) print_usage_info_and_exit("Two sort algorthm selected.\n");
            sort_algo = BUBBLE_SORT;
            sort_algo_param_found = true;
            param_ok = true;
        }
        if (0 == strcasecmp("-quicksort", argv[i])) {
            // quicksort selected
            if (sort_algo_param_found) print_usage_info_and_exit("Two sort algorthm selected.\n");
            sort_algo = QUICK_SORT;
            sort_algo_param_found = true;
            param_ok = true;
        }
        if ( 0 == strncasecmp(seed_str, argv[i], strlen(seed_str)) ) {
            // seed parameter found 
            if ( 1 == sscanf(argv[i]+strlen(seed_str), "%d", &seed) ) {
                if (seed_param_found) print_usage_info_and_exit("Two seed values defined.\n");
                seed_param_found = true;
                param_ok = true;
            }
        }
        if (!param_ok) print_usage_info_and_exit("Undefined parameter.\n"); // undefined parameter found
    } // for loop
}



int main(int argc, char **argv) {
    // scan parameter 

    program_name = argv[0];
    scan_params(argc, argv);
    printf("seed = %d sort algorithm = %s\n", seed, 
           (sort_algo == QUICK_SORT) ? "Quick Sort" : (sort_algo == BUBBLE_SORT) ? "Bubble Sort" : "undefined");
    fflush(stdout); 

    /* Fill memory with pseudo-random data */
    if (LENGTH <= 0) {
        fprintf(stderr, "LENGTH (array size) out of range");
        exit(EXIT_FAILURE); 
    }
    init_data(LENGTH);

    /* Display unsorted */
    printf("\nUnsorted:\n");
    display_data(LENGTH);

    /* Sort */
    printf("\nSorting:\n");
    sort(LENGTH);

    /* Display sorted */
    printf("\nSorted:\n");
    display_data(LENGTH);
    printf("\n");

    return 0;
}



void init_data(int length) {
    int i;
    int val;

    /* Init random generator */
    my_srand(seed);

    for(i = 0; i < length; i++) {
        val = my_rand() % RNDMOD;
        vmem_write(i, val);
    }   /* end for */
}



void display_data(int length) {
    int i;
    for(i = 0; i < length; i++) {
        int read = vmem_read(i);
        printf("%10d", read);
        printf("%c", ((i + 1) % NDISPLAYCOLS) ? ' ' : '\n');
    }   /* end for */
}



void sort(int length) {
    /* Quicksort */
    switch (sort_algo) {
       case QUICK_SORT :
           quicksort(0, length - 1);
           break;
       case BUBBLE_SORT :
           bubblesort(0, length - 1);
           break;
       default:
           fprintf(stderr, "Undefined sort algorithm in function sort");
           exit(EXIT_FAILURE); 
    }
}



static void bubblesort(int l, int r) {
    int i, j;
    for (i = l; i < r; i ++) {
        for (j = i + 1; j <= r; j ++) {
            if (vmem_read(j) < vmem_read(i)) {
               swap(i,j);
            }
        }
    }
}



void quicksort(int l, int r) {
    if(l < r) {
        int i = l;
        int j = r - 1;
        while(1) {      /* Put all elements < [r] to the left */
            while(vmem_read(i) < vmem_read(r)) {
                i++;
            }
            while((vmem_read(j) >= vmem_read(r)) && (j > l)) {
                j--;
            }
            if(i >= j) {
                break;
            }   /* end if */
            swap(i, j);
        }       /* end while */
        swap(i, r);     /* Put reference elemet to the boundary */
        /* Recursively sort the left and right half */
        quicksort(l, i - 1);
        quicksort(i + 1, r);
    }   /* end if */
}



void swap(int addr1, int addr2) {
    int tmp = vmem_read(addr1);
    vmem_write(addr1, vmem_read(addr2));
    vmem_write(addr2, tmp);
}



void print_usage_info_and_exit(char *err_str) {
    fprintf(stderr, "Wrong parameter: %s\n", err_str);
    fprintf(stderr, "Usage : %s [OPTIONS]\n", program_name);
    fprintf(stderr, " -quicksort : Use quicksort algorithm\n");
    fprintf(stderr, " -bubblesort : Use bubblesort algorithm\n");
    fprintf(stderr, " -seed=<int value> : Init randon number generator for generating the numbers\n");
    fprintf(stderr, "                     of the array to be sorted with <int value>\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
}


// EOF

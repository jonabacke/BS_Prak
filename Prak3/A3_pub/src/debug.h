/**
 * @file debug.h
 * @author Franz Korf, HAW Hamburg 
 * @date Nov 2014
 * @brief This header file defines some macros for debug output.
 */


#ifndef _DEBUG_H 
#define _DEBUG_H


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define  DEBUG_MESSAGES
#ifdef DEBUG_MESSAGES
/**
 * Print debug message when DEBUG_MESSAGES is defined.
 */
#define PRINT_DEBUG(str) { \
        fprintf str; \
	fflush(stderr); \
}
#else
#define PRINT_DEBUG(str) { }
#endif /* DEBUG_MESSAGES */


/**
 * Based on a condition this macro  generates an perror message an exits.
 */
#define TEST_AND_EXIT_ERRNO(cond, str) if (cond) { \
        perror(str); \
        exit(EXIT_FAILURE); \
}


/**
 * Based on a condition this macro prints an error message to stderr and exits.
 */
#define TEST_AND_EXIT(cond, str) if (cond) { \
        fprintf str; \
        exit(EXIT_FAILURE); \
}


#endif /* _DEBUG_H */


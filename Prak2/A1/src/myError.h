/*
 * myError.h
 *
 *  Created on: 28.04.2020
 *      Author: bs
 */

#ifndef MYERROR_H_
#define MYERROR_H_

/*
 * This header contrains error reporting marcos
 */


#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// This marco prints an error gererated by the command cmd.
// Standard Unix error handling and posix error handling will be supported.
#define HANDLE_ERR(cmd) \
{ int e = cmd; \
    if (e) { \
       printf( "Error in line %d of file %s: %s\n", __LINE__, __FILE__, strerror((e==-1)?errno:e)); \
       exit(1); \
    } \
}

#endif /* MYERROR_H_ */

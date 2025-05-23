#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

// Unity configuration for embedded testing

#define UNITY_INCLUDE_DOUBLE
#define UNITY_DOUBLE_PRECISION 1e-12

// Output formatting
#define UNITY_OUTPUT_CHAR(a)    putchar(a)
#define UNITY_OUTPUT_FLUSH()    fflush(stdout)
#define UNITY_OUTPUT_START()    
#define UNITY_OUTPUT_COMPLETE() 

// Memory management
#define UNITY_EXCLUDE_DETAILS
#define UNITY_EXCLUDE_SETJMP_H

// Test runner configuration
#define UNITY_INCLUDE_EXEC_TIME

#include <stdio.h>
#include <stdlib.h>

#endif // UNITY_CONFIG_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#ifdef __ANDROID__
    #include <android/log.h>
    #define PRINT_FN(...) __android_log_print(ANDROID_LOG_VERBOSE, "|", __VA_ARGS__)
#endif

#ifndef printf
int printf(const char* format, ...);
#define PRINT_FN(...) printf(__VA_ARGS__)
#endif

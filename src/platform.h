#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
    #include <unistd.h>
#endif

#define PRINT(...) {fprintf(stdout, __VA_ARGS__);}
#define WARN(...) {fprintf(stderr, __VA_ARGS__);}
#define ERROR(...) {fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE);}
#define ASSERT(b, ...) {if (!(b)) {ERROR(__VA_ARGS__);}}

#ifdef _WIN32

    #ifdef _MSC_VER
        #define _CRT_SECURE_NO_WARNINGS 1
    #endif

    #define strtok_r strtok_s
    #define strdup _strdup

    int64_t getline(char** line, size_t* len, FILE* fp);

#endif
//
// Created by itay on 11/18/21.
//

#include "utils.h"

void print_and_exit(char *msg, int code)
{
    fprintf(code ? stderr : stdout, "%s", msg);
    exit(code);
}

void *malloc_s(size_t size)
{
    void *p = malloc(size);
    if (!p) print_and_exit("Error: memory allocation error", EXIT_FAILURE);
    return p;
}
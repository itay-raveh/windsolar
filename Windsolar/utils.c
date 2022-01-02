//
// Created by itay on 11/18/21.
//

#include <stdlib.h>     // malloc()
#include "utils.h"
#include "macros.h"     // EXIT_WITH_MSG()

void *malloc_s(size_t size)
{
    void *p = malloc(size);
    if (p == NULL) EXIT_WITH_MSG(EXIT_FAILURE, "%s", "Error: memory allocation error");
    return p;
}

void *realloc_s(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);
    if (p == NULL) EXIT_WITH_MSG(EXIT_FAILURE, "%s", "Error: memory allocation error");
    return p;
}

char *newstr(char *const restrict src, const size_t len)
{
    char *str = (char *) malloc_s(len + 1);
    if (len > 0) strncpy(str, src, len);
    str[len] = '\0';
    return str;
}

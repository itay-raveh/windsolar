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

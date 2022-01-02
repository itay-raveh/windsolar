//
// Created by itay on 11/18/21.
//

#ifndef WINDSOLAR_UTILS_H
#define WINDSOLAR_UTILS_H

#include <stddef.h>     // size_t

/**
 * Safe malloc. will exit with error when allocation fails
 *
 * @param size - size of new memory block
 * @return ptr to memory block
 */
void *malloc_s(size_t size);

#define NEW(T) ((T*)malloc_s(sizeof(T)))

/**
 * Safe realloc. will exit with error when allocation fails
 *
 * @param ptr - memory block
 * @param size - new size of memory block
 * @return ptr to memory block
 */
void *realloc_s(void *ptr, size_t size);

/**
 * Dynamically a allocate a new string as a copy of the source
 *
 * @param src - buffer to copy from
 * @param len - length of string
 * @return ptr to the new string
 */
char *newstr(char *restrict src, size_t len);

#endif //WINDSOLAR_UTILS_H

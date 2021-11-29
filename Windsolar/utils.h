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

#define MALLOC(T) (T*)malloc_s(sizeof(T))

#endif //WINDSOLAR_UTILS_H

//
// Created by itay on 11/18/21.
//

#ifndef WINDSOLAR_UTILS_H
#define WINDSOLAR_UTILS_H

#include <stdlib.h>

/**
 * Safe malloc. will exit with error when allocation failes
 *
 * @param size - size of new memory block
 * @return ptr to memory block
 */
void *malloc_s(size_t size);

#endif //WINDSOLAR_UTILS_H

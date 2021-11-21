//
// Created by itay on 11/18/21.
//

#ifndef WINDSOLAR_MACROS_H
#define WINDSOLAR_MACROS_H

#ifdef DEBUG
#include <stdio.h>
#define TRACE(fmt, ...) do{fprintf(stderr, "%s:%s(%4u) " fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);}while(0)
#else
#define TRACE(fmt, ...)
#endif

#endif //WINDSOLAR_MACROS_H

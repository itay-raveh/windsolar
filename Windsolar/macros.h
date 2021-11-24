//
// Created by itay on 11/18/21.
//

#ifndef WINDSOLAR_MACROS_H
#define WINDSOLAR_MACROS_H

#include <stdio.h>

#define EXIT_WITH_MSG(code, fmt, ...) do{fprintf(code ? stderr : stdout, fmt, __VA_ARGS__);exit(code);}while(0)

#ifdef DEBUG
#define TRACE(fmt, ...) do{fprintf(stderr, "[%s:%s\t:%3u]\t" fmt, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);}while(0)
#else
#define TRACE(fmt, ...)
#endif

#endif //WINDSOLAR_MACROS_H

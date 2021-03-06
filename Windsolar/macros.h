//
// Created by itay on 11/18/21.
//

#ifndef WINDSOLAR_MACROS_H
#define WINDSOLAR_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR2(s) #s
#define STR(s) STR2(s)

#ifdef DEBUG
#define TRACE(fmt, ...) do{ \
                        fprintf(stderr, \
                            "%s:%u:%s%*s" fmt, \
                            __FILE__, __LINE__, __FUNCTION__,\
                            69 /*lol*/ - (int)(strlen(__FILE__) + strlen(__FUNCTION__) + strlen(STR(__LINE__))), \
                            " ",\
                            __VA_ARGS__); \
                        }while(0)
#else
#define TRACE(fmt, ...)
#endif

#define EXIT_WITH_MSG(code, fmt, ...) do{fprintf(code ? stderr : stdout, fmt, __VA_ARGS__);exit(code);}while(0)

#endif //WINDSOLAR_MACROS_H

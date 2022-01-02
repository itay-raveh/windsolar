//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_STACKS_H
#define WINDSOLAR_STACKS_H

#include <stdbool.h>
#include "utils.h"  // NEW(), realloc_s(), malloc_s()
#include "token.h"

#define STACK_DECL(typename, type) \
    typedef struct { \
        size_t len; \
        size_t capacity; \
        type* data; \
    } typename; \
    typename *typename##_new(); \
    void typename##_free(typename *stck); \
    void typename##_push(typename *stck, type value); \
    type typename##_pop(typename *stck);

typedef struct data_frame_t
{
    bool is_number;
    double number;
    char *str;
} DataFrame;

STACK_DECL(DataStack, DataFrame)

//void DataStack_print(DataStack *ds, int limit);

typedef struct program_frame_t
{
    Token type;
    char *str;
} ProgramFrame;

STACK_DECL(ProgramStack, ProgramFrame)

#endif //WINDSOLAR_STACKS_H

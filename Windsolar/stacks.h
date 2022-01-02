//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_STACKS_H
#define WINDSOLAR_STACKS_H

#include <stdbool.h>
#include <stdlib.h> // free()
#include <assert.h> // assert()
#include "utils.h"  // NEW(), realloc_s(), malloc_s()
#include "token.h"

#define DECL_STACK(typename, type) \
    typedef struct { \
        size_t len;         /* amount of data currently in the stack */ \
        size_t capacity;    /* amount of data possible in the stack */ \
        type* data;         /* data array */ \
    } typename; \
 \
    typename *typename##_new() { \
        typename *s = NEW(typename); \
        s->len = 0; \
        s->capacity = 1; \
        s->data = malloc_s(sizeof(type)); \
        return s; \
    } \
 \
    void typename##_free(typename* stck) { \
        free(stck->data); \
        free(stck); \
    } \
 \
    void typename##_push(typename* stck, type value) { \
        assert(stck); \
 \
        if (++(stck->len) > stck->capacity) { \
            stck->capacity = stck->capacity * 2; \
            stck->data = realloc_s(stck->data, stck->capacity * sizeof(type)); \
        } \
 \
        stck->data[stck->len - 1] = value; \
    } \
 \
    type typename##_pop(typename* stck) { \
        assert(stck); \
        assert(stck->len > 0); \
 \
        stck->len--; \
        return stck->data[stck->len]; \
    }

typedef struct data_frame_t
{
    bool is_number;
    double number;
    char *str;
} DataFrame;

DECL_STACK(DataStack, DataFrame)

typedef struct program_frame_t
{
    Token type;
    char *str;
} ProgramFrame;

DECL_STACK(ProgramStack, ProgramFrame)

#endif //WINDSOLAR_STACKS_H

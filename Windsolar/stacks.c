//
// Created by itay on 1/2/22.
//

#include <stdlib.h> // free()
#include <assert.h> // assert()
#include "stacks.h"

#define STACK_IMPL(typename, type) \
    typename *typename##_new() \
    { \
        typename *s = NEW(typename); \
        s->len = 0; \
        s->capacity = 1; \
        s->data = malloc_s(sizeof(type)); \
        return s; \
    } \
 \
    void typename##_free(typename* stck)  \
    { \
        free(stck->data); \
        free(stck); \
    } \
 \
    void typename##_push(typename* stck, type value) \
    { \
        assert(stck); \
 \
        if (++(stck->len) > stck->capacity) \
        { \
            stck->capacity = stck->capacity * 2; \
            stck->data = realloc_s(stck->data, stck->capacity * sizeof(type)); \
        } \
 \
        stck->data[stck->len - 1] = value; \
    } \
 \
    type typename##_pop(typename* stck)   \
    { \
        assert(stck); \
        assert(stck->len > 0); \
 \
        stck->len--; \
        return stck->data[stck->len]; \
    }

//int size = (limit > stck->len ? stck-> len : limit); \
//        for (int i = 0; i < size; i++) printf("+---------------"); \
//        printf("+"); \
//                                   \
//        for (int i = 0; i < size; i++) printf("+---------------"); \
//        printf("+"); \

STACK_IMPL(DataStack, DataFrame)

STACK_IMPL(ProgramStack, ProgramFrame)

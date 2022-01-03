//
// Created by itay on 1/2/22.
//

#include <stdlib.h> // free()
#include <stdio.h>
#include <assert.h> // assert()
#include <string.h>
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


STACK_IMPL(DataStack, DataFrame)

STACK_IMPL(ProgramStack, ProgramFrame)

#define FRAME_W 16
#define CONTENT_W (FRAME_W - 2)
#define MIN(val, cap) (val > cap ? cap : val)

char *string_repr(char *src, size_t len, size_t max_len)
{
    char *str = (char *) malloc_s(len + 3);
    size_t new_len = len + 2;

    if (new_len <= max_len) strncpy(&(str[1]), src, len);
    else
    {
        strncpy(&(str[1]), src, max_len);
        new_len = max_len;
        str[new_len - 2] = str[new_len - 3] = str[new_len - 4] = '.';
    }

    str[new_len - 1] = str[0] = '"';
    str[new_len] = '\0';

    return str;
}

void DataStack_print(DataStack *ds, size_t limit)
{
    size_t size = (limit > ds->len ? ds->len : limit);

    puts("DATA STACK");
    printf("T ");
    for (int i = 0; i < size; i++)
    {
        printf("+");
        for (int j = 0; j < FRAME_W; j++)
            printf("-");
    }
    puts("+");
    printf("O ");

    for (int i = (int)size - 1; i >= 0; i--)
    {
        if (ds->data[i].is_number)
        {
            char str[FRAME_W - 2];
            if ((int) ds->data[i].number == ds->data[i].number) sprintf(str, "%d", (int) ds->data[i].number);
            else sprintf(str, "%f", ds->data[i].number);
            printf("| %*s ", -CONTENT_W, str);
        } else
        {
            size_t orig_str_len = strlen(ds->data[i].str);
            size_t capped_str_len = MIN(orig_str_len, CONTENT_W);
            char *str = string_repr(ds->data[i].str, capped_str_len, CONTENT_W);
            printf("| %*s ", -CONTENT_W, str);
            free(str);
        }
    }

    puts("|");
    printf("P ");
    for (int i = 0; i < size; i++)
    {
        printf("+");
        for (int j = 0; j < FRAME_W; j++)
            printf("-");
    }
    puts("+");
}

void ProgramStack_print(ProgramStack *ps, size_t limit)
{
    size_t size = MIN(ps->len, limit);

    puts("PROGRAM STACK");
    printf("T ");
    for (int i = 0; i < size; i++)
    {
        printf("+");
        for (int j = 0; j < FRAME_W; j++)
            printf("-");
    }
    puts("+");
    printf("O ");

    for (int i = (int)size - 1; i >= 0; i--)
    {
        size_t orig_str_len = strlen(ps->data[i].str);
        size_t capped_str_len = MIN(orig_str_len, CONTENT_W);

        char *str;
        if (ps->data[i].type == T_STRING) str = string_repr(ps->data[i].str, capped_str_len, CONTENT_W);
        else str = newstr(ps->data[i].str, capped_str_len);

        printf("| %*s ", -CONTENT_W, str);
        free(str);
    }

    puts("|");
    printf("P ");
    for (int i = 0; i < size; i++)
    {
        printf("+");
        for (int j = 0; j < FRAME_W; j++)
            printf("-");
    }
    puts("+");
}

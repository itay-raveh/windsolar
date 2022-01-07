//
// Created by itay on 1/2/22.
//

#include <stdlib.h> // free()
#include <stdio.h>
#include <assert.h> // assert()
#include <inttypes.h>
#include "utils.h"  // NEW(), realloc_s(), malloc_s()
#include "stack.h"

Stack *Stack_new()
{
    Stack *s = NEW(Stack);
    s->len = 0;
    s->capacity = 1;
    s->data = malloc_s(sizeof(void *));
    return s;
}

void Stack_free(Stack *s, void (*free_item)(void *))
{
    for (size_t i = 0; i < s->len; i++) free_item(s->data[i]);
    free(s->data);
    free(s);
}

void Stack_push(Stack *s, void *frame)
{
    assert(s);
    assert(frame);

    if (++(s->len) > s->capacity)
    {
        s->capacity = s->capacity * 2;
        s->data = realloc_s(s->data, s->capacity * sizeof(void *));
    }

    s->data[s->len - 1] = frame;
}

void *Stack_pop(Stack *s)
{
    assert(s);
    assert(s->len > 0);

    return s->data[--(s->len)];
}

void Stack_print(Stack *s, const size_t frame_width, const size_t frames, void (*print_item)(void *, int16_t))
{
    assert(s);

    const uint32_t first = s->len - 1;
    const uint32_t last = s->len > frames ? s->len - frames : 0;
    const int16_t content_width = frame_width - 2;

    printf("T ");
    for (uint32_t i = first; i >= last; i--)
    {
        printf("+");
        for (uint8_t j = 0; j < content_width; j++)
            printf("-");
    }
    printf("+\nO");

    for (uint32_t i = first; i >= last; i--)
    {
        printf(" | ");
        print_item(s->data[i], content_width);
    }

    printf("|\nP ");
    for (uint32_t i = first; i >= last; i--)
    {
        printf("+");
        for (uint8_t j = 0; j < content_width; j++)
            printf("-");
    }
    puts("+");
}

//char *string_repr(char *src, size_t len, size_t max_len)
//{
//    char *str = (char *) malloc_s(len + 3);
//    size_t new_len = len + 2;
//
//    if (new_len <= max_len) strncpy(&(str[1]), src, len);
//    else
//    {
//        strncpy(&(str[1]), src, max_len);
//        new_len = max_len;
//        str[new_len - 2] = str[new_len - 3] = str[new_len - 4] = '.';
//    }
//
//    str[new_len - 1] = str[0] = '"';
//    str[new_len] = '\0';
//
//    return str;
//}

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

void Stack_print(Stack *s, const uint16_t frame_width, const uint64_t frames, void (*print_item)(void *, uint16_t))
{
    assert(s);

    const int32_t first = s->len - 1;
    const int32_t last = (s->len > frames ? s->len - frames: 0) - 1;
    const uint16_t content_width = frame_width - 2;

    printf("T ");
    for (int32_t i = first; i > last; i--)
    {
        printf("+");
        for (uint16_t j = 0; j < frame_width; j++)
            printf("-");
    }
    printf("+\nO");

    for (int32_t i = first; i > last; i--)
    {
        printf(" | ");
        print_item(s->data[i], content_width);
    }

    printf(" |\nP ");
    for (int32_t i = first; i > last; i--)
    {
        printf("+");
        for (uint16_t j = 0; j < frame_width; j++)
            printf("-");
    }
    puts("+");
}

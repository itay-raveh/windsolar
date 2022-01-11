//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_STACK_H
#define WINDSOLAR_STACK_H

#include <inttypes.h>

typedef struct
{
    uint64_t len;
    uint64_t capacity;
    void **data;
} Stack;

Stack *Stack_new();

void Stack_free(Stack *restrict s, void (*free_item)(void *));

void Stack_push(Stack *restrict s, void *restrict frame);

void *Stack_pop(Stack *restrict s);

void Stack_print(Stack *restrict s, uint16_t frame_width, uint64_t frames, void (*print_item)(void *, uint16_t));

#endif //WINDSOLAR_STACK_H

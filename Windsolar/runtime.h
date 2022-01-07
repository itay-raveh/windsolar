//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_RUNTIME_H
#define WINDSOLAR_RUNTIME_H

#include "stack.h"
#include "parse_tree.h"

bool mainloop(LabelNode *pt, Stack *ps, Stack *ds, bool print_stacks, size_t frames);

#endif //WINDSOLAR_RUNTIME_H

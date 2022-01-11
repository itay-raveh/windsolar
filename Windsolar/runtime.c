//
// Created by itay on 1/2/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"
#include "commands.h"
#include "frames.h"
#include "utils.h"


#define FRAME_W 16

bool mainloop(LabelNode *restrict pt, Stack *const restrict ps, Stack *const restrict ds, const bool print_stacks,
              uint64_t frames)
{
    DataFrame *main = DataFrame_new(NULL, newstr("MAIN", 4));
    Stack_push(ds, main);

    if (!execCommand(pt, ps, ds, "CALL"))
    {
        fprintf(stderr, "You must define a 'MAIN' subroutine where execution can start\n");
        return false;
    }

    while (ps->len > 0)
    {
        if (print_stacks)
        {
            puts("\nDATA STACK");
            Stack_print(ds, FRAME_W, frames, (void (*)(void *, uint16_t)) DataFrame_print);
            puts("PROGRAM STACK");
            Stack_print(ps, FRAME_W, frames, (void (*)(void *, uint16_t)) ProgramFrame_print);
            puts("");
        }

        ProgramFrame *pf = Stack_pop(ps);
        if (pf->type == T_NUMBER)
        {
            char *end;
            double number = strtod(pf->str, &end);
            DataFrame *df = DataFrame_new(&number, NULL);
            Stack_push(ds, df);
        } else if (pf->type == T_STRING)
        {
            DataFrame *df = DataFrame_new(NULL, newstr(pf->str, strlen(pf->str)));
            Stack_push(ds, df);
        } else if (pf->type == T_NAME)
        {
            if (!execCommand(pt, ps, ds, pf->str)) return false;
        }
        ProgramFrame_free(pf);
    }
    return true;
}

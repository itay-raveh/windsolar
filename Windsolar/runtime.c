//
// Created by itay on 1/2/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "runtime.h"
#include "commands.h"

bool mainloop(LabelNode *pt, ProgramStack *ps, DataStack *ds)
{
    DataFrame main = {.is_number = false, .str = "MAIN"};
    DataStack_push(ds, main);

    DataStack_print(ds, 5);
    ProgramStack_print(ps, 5);

    if (!execCommand(pt, ps, ds, "CALL"))
    {
        fprintf(stderr, "You must define a 'MAIN' subroutine where execution can start\n");
        return false;
    }

    while (ps->len > 0)
    {

        puts("");
        DataStack_print(ds, 5);
        ProgramStack_print(ps, 5);

        ProgramFrame p = ProgramStack_pop(ps);
        if (p.type == T_NUMBER)
        {
            char *end;
            DataFrame d = {.is_number = true, .number = strtod(p.str, &end)};
            DataStack_push(ds, d);
        } else if (p.type == T_STRING)
        {
            DataFrame d = {.is_number = false, .str = p.str};
            DataStack_push(ds, d);
        } else if (p.type == T_NAME)
        {
            if (!execCommand(pt, ps, ds, p.str)) return false;
        }
    }
    return true;
}

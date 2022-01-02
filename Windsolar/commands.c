//
// Created by itay on 1/2/22.
//

#include <stdio.h>
#include <string.h>
#include "commands.h"


void printCommandRuntimeError(char *command, char *msg)
{
    fprintf(stderr, "Runtime Error: %s: %s\n", command, msg);
}

bool CALL(LabelNode *pt, ProgramStack *ps, DataStack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("CALL", "At least 1 item must be present in the DataStack");
        return false;
    }

    DataFrame d = DataStack_pop(ds);
    if (d.is_number)
    {
        printCommandRuntimeError("CALL", "Can not CALL a number");
        return false;
    }

    while (pt != NULL && strcmp(pt->label, d.str) != 0)
        pt = pt->next;

    if (pt == NULL)
    {
        char str[100];
        sprintf(str, "No subroutine named %s", d.str);
        printCommandRuntimeError("CALL", str);
        return false;
    }

    ProgramStack *reverse = ProgramStack_new();
    for (InstNode *inst = pt->block_head; inst != NULL; inst = inst->next)
    {
        ProgramFrame pf = {.type = inst->type, .str = inst->str};
        ProgramStack_push(reverse, pf);
    }
    do ProgramStack_push(ps, ProgramStack_pop(reverse)); while (reverse->len > 0);

    ProgramStack_free(reverse);
    return true;
}

bool WRITE(DataStack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("WRITE", "At least 1 item must be present in the DataStack");
        return false;
    }

    DataFrame d = DataStack_pop(ds);
    if (d.is_number)
    {
        if (d.number == (int64_t) d.number) printf("%ld", (int64_t) d.number);
        else printf("%f", d.number);
    } else printf("%s", d.str);

    return true;
}

bool execCommand(LabelNode *pt, ProgramStack *ps, DataStack *ds, char *command)
{
    #define IS(s) (strcmp(command, s) == 0)

    if (IS("CALL")) return CALL(pt, ps, ds);
    if (IS("WRITE")) return WRITE(ds);

    printCommandRuntimeError(command, "No such command");
    return false;
}

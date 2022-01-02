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

    if (IS("WRITE"))
    {
        return WRITE(ds);
    }

    printCommandRuntimeError(command, "No such command");
    return false;
}

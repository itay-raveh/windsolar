//
// Created by itay on 1/2/22.
//

#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "frames.h"
#include "utils.h"


void printCommandRuntimeError(char *command, char *msg)
{
    fprintf(stderr, "Runtime Error: %s: %s\n", command, msg);
}

bool CALL(LabelNode *pt, Stack *ps, Stack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("CALL", "At least 1 item must be present in the Stack");
        return false;
    }

    DataFrame *df = Stack_pop(ds);
    if (df->isNumber)
    {
        printCommandRuntimeError("CALL", "Can not CALL a number");
        return false;
    }

    while (pt != NULL && strcmp(pt->label, df->str) != 0)
        pt = pt->next;

    if (pt == NULL)
    {
        char str[100] = "No subroutine named ";
        strcat(str, df->str);
        printCommandRuntimeError("CALL", str);
        return false;
    }

    DataFrame_free(df);

    Stack *reverse = Stack_new();
    for (InstNode *inst = pt->blockHead; inst != NULL; inst = inst->next)
    {
        ProgramFrame *pf = ProgramFrame_new(inst->type, newstr(inst->str, strlen(inst->str)));
        Stack_push(reverse, pf);
    }
    do Stack_push(ps, Stack_pop(reverse)); while (reverse->len > 0);

    Stack_free(reverse, (void (*)(void *)) ProgramFrame_free);
    return true;
}

bool WRITE(Stack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("WRITE", "At least 1 item must be present in the Stack");
        return false;
    }

    DataFrame *df = Stack_pop(ds);
    if (df->isNumber)
    {
        if ((int64_t) df->number == df->number)
            printf("%ld", (int64_t) df->number);
        else
            printf("%f", df->number);
    } else
    {
        char *p = df->str;
        while (*p != '\0')
        {
            if (*p == '\\')
            {
                char c;
                switch (*(p + 1))
                {
                    case 'a': c = '\a';
                        break;
                    case 'b': c = '\b';
                        break;
                    case 'f': c = '\f';
                        break;
                    case 'n': c = '\n';
                        break;
                    case 'r': c = '\r';
                        break;
                    case 't': c = '\t';
                        break;
                }
                putc(c, stdout);
                p += 2;
            } else
            {
                putc(*p, stdout);
                p++;
            }
        }
    }

    DataFrame_free(df);
    return true;
}

#define MAX_INPUT_LEN 100

bool READ(Stack *ds)
{
    char *inp = malloc_s(MAX_INPUT_LEN);
    if (fgets(inp, MAX_INPUT_LEN, stdin) == NULL)
    {
        printCommandRuntimeError("READ", "Could not read user input");
        return false;
    }
    *strchr(inp, '\n') = '\0';

    DataFrame *df = DataFrame_new(NULL, inp);
    Stack_push(ds, df);

    return true;
}

bool execCommand(LabelNode *pt, Stack *ps, Stack *ds, char *command)
{
    #define IS(s) (strcmp(command, s) == 0)

    if (IS("CALL")) return CALL(pt, ps, ds);
    if (IS("WRITE")) return WRITE(ds);
    if (IS("READ")) return READ(ds);

    printCommandRuntimeError(command, "No such command");
    return false;
}

//
// Created by itay on 1/2/22.
//

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <Windows.h>
#else

#include <unistd.h>

#endif

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
        printCommandRuntimeError("CALL", "At least 1 item must be present in the Data Stack");
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

bool ADD(Stack *ds)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError("ADD", "At least 2 items must be present in the Data Stack");
        return false;
    }

    DataFrame *df1 = Stack_pop(ds);
    if (!df1->isNumber)
    {
        printCommandRuntimeError("ADD", "Can not ADD on a string");
        return false;
    }
    DataFrame *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError("ADD", "Can not ADD on a string");
        return false;
    }

    double res = df2->number + df1->number;
    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df1);
    DataFrame_free(df2);
    return true;
}

bool SUB(Stack *ds)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError("SUB", "At least 2 items must be present in the Data Stack");
        return false;
    }

    DataFrame *df1 = Stack_pop(ds);
    if (!df1->isNumber)
    {
        printCommandRuntimeError("SUB", "Can not SUB on a string");
        return false;
    }
    DataFrame *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError("SUB", "Can not SUB on a string");
        return false;
    }

    double res = df2->number - df1->number;
    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df1);
    DataFrame_free(df2);
    return true;
}

bool MUL(Stack *ds)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError("MUL", "At least 2 items must be present in the Data Stack");
        return false;
    }

    DataFrame *df1 = Stack_pop(ds);
    if (!df1->isNumber)
    {
        printCommandRuntimeError("MUL", "Can not MUL on a string");
        return false;
    }
    DataFrame *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError("MUL", "Can not MUL on a string");
        return false;
    }

    double res = df2->number * df1->number;
    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df1);
    DataFrame_free(df2);
    return true;
}

bool DIV(Stack *ds)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError("DIV", "At least 2 items must be present in the Data Stack");
        return false;
    }

    DataFrame *df1 = Stack_pop(ds);
    if (!df1->isNumber)
    {
        printCommandRuntimeError("DIV", "Can not DIV on a string");
        return false;
    }
    DataFrame *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError("DIV", "Can not DIV on a string");
        return false;
    }

    double res = df2->number / df1->number;
    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df1);
    DataFrame_free(df2);
    return true;
}

bool DUP(Stack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("DUP", "At least 1 item must be present in the Data Stack");
        return false;
    }

    DataFrame *df = Stack_pop(ds), *df_copy;
    if (df->isNumber) df_copy = DataFrame_new(&df->number, NULL);
    else df_copy = DataFrame_new(NULL, df->str);

    Stack_push(ds, df);
    Stack_push(ds, df_copy);

    return true;
}

bool WRITE(Stack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("WRITE", "At least 1 item must be present in the Data Stack");
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

bool SLEEP(Stack *ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("SLEEP", "At least 1 item must be present in the Data Stack");
        return false;
    }

    DataFrame *df = Stack_pop(ds);
    if (!df->isNumber)
    {
        printCommandRuntimeError("SLEEP", "Can not SLEEP on string");
        return false;
    }

    #ifdef WIN32
    Sleep(pollingDelay);
    #else
    usleep(df->number * 1000 * 1000);
    #endif

    return true;
}

bool execCommand(LabelNode *pt, Stack *ps, Stack *ds, char *command)
{
    #define IS(s) (strcmp(command, s) == 0)

    if (IS("CALL")) return CALL(pt, ps, ds);
    if (IS("ADD")) return ADD(ds);
    if (IS("SUB")) return SUB(ds);
    if (IS("MUL")) return MUL(ds);
    if (IS("DIV")) return DIV(ds);
    if (IS("DUP")) return DUP(ds);
    if (IS("WRITE")) return WRITE(ds);
    if (IS("READ")) return READ(ds);
    if (IS("SLEEP")) return SLEEP(ds);

    printCommandRuntimeError(command, "No such command");
    return false;
}

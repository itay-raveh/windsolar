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

#define DATA_COUNT_MSG(exp) "Expected at least " #exp " items in the Data Stack, found less"
#define DATA_TYPE_MSG(exp, found) "Expected Data of type " #exp ", found " #found

bool CALL(LabelNode *restrict pt, Stack *const restrict ps, Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("CALL", DATA_COUNT_MSG(1));
        return false;
    }

    DataFrame *df = Stack_pop(ds);
    if (df->isNumber)
    {
        printCommandRuntimeError("CALL", DATA_TYPE_MSG("STRING", "NUMBER"));
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

bool CCALL(LabelNode *restrict pt, Stack *const restrict ps, Stack *const restrict ds)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError("CCALL", DATA_COUNT_MSG(2));
        return false;
    }

    DataFrame *df1 = Stack_pop(ds), *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError("CCALL", DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }

    if (df2->number != 0)
    {
        Stack_push(ds, df1);
        DataFrame_free(df2);
        return CALL(pt, ps, ds);
    } else
    {
        DataFrame_free(df1);
        DataFrame_free(df2);
        return true;
    }
}

bool BRANCH(LabelNode *restrict pt, Stack *const restrict ps, Stack *const restrict ds)
{
    if (ds->len < 3)
    {
        printCommandRuntimeError("BRANCH", DATA_COUNT_MSG(3));
        return false;
    }

    DataFrame *df1 = Stack_pop(ds), *df2 = Stack_pop(ds), *df3 = Stack_pop(ds);
    if (!df3->isNumber)
    {
        printCommandRuntimeError("BRANCH", DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }

    if (df3->number != 0)
    {
        Stack_push(ds, df2);
        DataFrame_free(df1);
    } else
    {
        Stack_push(ds, df1);
        DataFrame_free(df2);
    }

    DataFrame_free(df3);
    return CALL(pt, ps, ds);
}

bool BINARY_OP(Stack *const restrict ds, char *const restrict op)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError(op, DATA_COUNT_MSG(2));
        return false;
    }

    DataFrame *df1 = Stack_pop(ds);
    if (!df1->isNumber)
    {
        printCommandRuntimeError(op, DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }
    DataFrame *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError(op, DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }

    double res;
    if (strcmp(op, "ADD") == 0) res = df2->number + df1->number;
    else if (strcmp(op, "SUB") == 0) res = df2->number - df1->number;
    else if (strcmp(op, "MUL") == 0) res = df2->number * df1->number;
    else if (strcmp(op, "DIV") == 0) res = df2->number / df1->number;
    else if (strcmp(op, "MOD") == 0) res = (int32_t) df2->number % (int32_t) df1->number;

    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df1);
    DataFrame_free(df2);
    return true;
}

bool BINARY_CMP(Stack *const restrict ds, char *const restrict cmp)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError(cmp, DATA_COUNT_MSG(2));
        return false;
    }

    DataFrame *df1 = Stack_pop(ds);
    if (!df1->isNumber)
    {
        printCommandRuntimeError(cmp, DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }
    DataFrame *df2 = Stack_pop(ds);
    if (!df2->isNumber)
    {
        printCommandRuntimeError(cmp, DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }

    double res;
    if (strcmp(cmp, "EQ") == 0) res = df2->number == df1->number;
    else if (strcmp(cmp, "NE") == 0) res = df2->number != df1->number;
    else if (strcmp(cmp, "GT") == 0) res = df2->number > df1->number;
    else if (strcmp(cmp, "GE") == 0) res = df2->number >= df1->number;
    else if (strcmp(cmp, "LT") == 0) res = df2->number < df1->number;
    else if (strcmp(cmp, "LE") == 0) res = df2->number <= df1->number;
    else if (strcmp(cmp, "AND") == 0) res = df2->number && df1->number;
    else if (strcmp(cmp, "OR") == 0) res = df2->number || df1->number;
    else return false;

    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df1);
    DataFrame_free(df2);
    return true;
}

bool NOT(Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("NOT", DATA_COUNT_MSG(1));
        return false;
    }

    DataFrame *df = Stack_pop(ds);
    if (!df->isNumber)
    {
        printCommandRuntimeError("NOT", DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }

    double res = !df->number;
    Stack_push(ds, DataFrame_new(&res, NULL));

    DataFrame_free(df);
    return true;
}

bool POP(Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("POP", DATA_COUNT_MSG(1));
        return false;
    }

    DataFrame_free(Stack_pop(ds));
    return true;
}

bool DUP(Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("DUP", DATA_COUNT_MSG(1));
        return false;
    }

    DataFrame *df = Stack_pop(ds), *df_copy;
    if (df->isNumber) df_copy = DataFrame_new(&df->number, NULL);
    else df_copy = DataFrame_new(NULL, df->str);

    Stack_push(ds, df);
    Stack_push(ds, df_copy);

    return true;
}

bool DUP2(Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("DUP", DATA_COUNT_MSG(1));
        return false;
    }

    DataFrame *df1 = Stack_pop(ds), *df1_copy, *df2 = Stack_pop(ds), *df2_copy;
    if (df1->isNumber) df1_copy = DataFrame_new(&df1->number, NULL);
    else df1_copy = DataFrame_new(NULL, df1->str);
    if (df2->isNumber) df2_copy = DataFrame_new(&df2->number, NULL);
    else df2_copy = DataFrame_new(NULL, df2->str);

    Stack_push(ds, df2);
    Stack_push(ds, df1);
    Stack_push(ds, df2_copy);
    Stack_push(ds, df1_copy);

    return true;
}

bool SWAP12(Stack *const restrict ds)
{
    if (ds->len < 2)
    {
        printCommandRuntimeError("SWAP12", DATA_COUNT_MSG(2));
        return false;
    }

    DataFrame *first = Stack_pop(ds), *second = Stack_pop(ds);
    Stack_push(ds, first);
    Stack_push(ds, second);

    return true;
}

bool SWAP13(Stack *const restrict ds)
{
    if (ds->len < 3)
    {
        printCommandRuntimeError("SWAP13", DATA_COUNT_MSG(3));
        return false;
    }

    DataFrame *first = Stack_pop(ds), *second = Stack_pop(ds), *third = Stack_pop(ds);
    Stack_push(ds, first);
    Stack_push(ds, second);
    Stack_push(ds, third);

    return true;
}

bool WRITE(Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("WRITE", DATA_COUNT_MSG(1));
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

bool READ(Stack *const restrict ds)
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

bool SLEEP(Stack *const restrict ds)
{
    if (ds->len < 1)
    {
        printCommandRuntimeError("SLEEP", DATA_COUNT_MSG(1));
        return false;
    }

    DataFrame *df = Stack_pop(ds);
    if (!df->isNumber)
    {
        printCommandRuntimeError("SLEEP", DATA_TYPE_MSG("NUMBER", "STRING"));
        return false;
    }

    #ifdef WIN32
    Sleep(pollingDelay * 1000);
    #else
    usleep(df->number * 1000 * 1000);
    #endif

    return true;
}

bool
execCommand(LabelNode *restrict pt, Stack *const restrict ps, Stack *const restrict ds, char *const restrict command)
{
    #define IS(s) (strcmp(command, s) == 0)

    if (IS("CALL")) return CALL(pt, ps, ds);
    if (IS("CCALL")) return CCALL(pt, ps, ds);
    if (IS("BRANCH")) return BRANCH(pt, ps, ds);
    if (IS("ADD") || IS("SUB") || IS("MUL") || IS("DIV") || IS("MOD")) return BINARY_OP(ds, command);
    if (IS("NOT")) return NOT(ds);
    if (IS("EQ") || IS("NE") || IS("GT") || IS("GE") || IS("LT") || IS("LE") || IS("AND") || IS("OR"))
        return BINARY_CMP(ds, command);
    if (IS("POP")) return POP(ds);
    if (IS("DUP")) return DUP(ds);
    if (IS("DUP2")) return DUP2(ds);
    if (IS("SWAP12")) return SWAP12(ds);
    if (IS("SWAP13")) return SWAP13(ds);
    if (IS("WRITE")) return WRITE(ds);
    if (IS("READ")) return READ(ds);
    if (IS("SLEEP")) return SLEEP(ds);

    printCommandRuntimeError(command, "No such command");
    return false;
}

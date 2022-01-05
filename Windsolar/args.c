//
// Created by itay on 1/3/22.
//

#include <stdlib.h>
#include <string.h>
#include "args.h"
#include "utils.h"

Args *Args_fromArgv(uint8_t argc, char *argv[])
{
    Args *a = NEW(Args);
    a->inputFileName = argv[1];
    a->printTokens = false;
    a->printParseTree = false;
    a->printStacks = false;
    a->stackPrintLength = 0;

    for (uint8_t i = 2; i < argc; i++)
    {
        if (argv[i][0] != '-') return NULL;
        if (strcmp(argv[i], "--print-tokens") == 0 || strcmp(argv[i], "-t") == 0) a->printTokens = true;
        if (strcmp(argv[i], "--print-parse-tree") == 0 || strcmp(argv[i], "-p") == 0) a->printParseTree = true;
        if (strcmp(argv[i], "--print-stacks") == 0 || strcmp(argv[i], "-s") == 0)
        {
            a->printStacks = true;
            if (i > argc - 2) return NULL;
            char *end;
            a->stackPrintLength = strtol(argv[i + 1], &end, 10);
            i++;
        }
    }
    return a;
}

void Args_free(Args *a)
{
    free(a->inputFileName);
    free(a);
}

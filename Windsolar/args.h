//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_ARGS_H
#define WINDSOLAR_ARGS_H

#include <stdbool.h>
#include <inttypes.h>

typedef struct
{
    char* inputFileName;
    bool printTokens;
    bool printParseTree;
    bool printStacks;
    uint8_t stackPrintLength;
} Args;

Args *Args_fromArgv(uint8_t argc, char *argv[]);
void Args_free(Args *a);

#endif //WINDSOLAR_ARGS_H

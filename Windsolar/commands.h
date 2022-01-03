//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_COMMANDS_H
#define WINDSOLAR_COMMANDS_H

#include <stdbool.h>
#include "stacks.h"
#include "parse_tree.h"

/* ------------------- Flow Control ------------------- */
bool CALL(LabelNode *pt, ProgramStack *ps, DataStack *ds);

/* ------------------------ IO ------------------------ */
/* pop the top element from the ds and print it */
bool WRITE(DataStack *ds);
/* read input from the user and push it to the ds */
bool READ(DataStack *ds);

/**
 * Execute a given command. If execution fails, a runtime error will be printed.
 *
 * @param pt - head of the parse tree
 * @param ps - program stack
 * @param ds - data stack
 * @param command - command to execute
 * @return whether execution was successful
 */
bool execCommand(LabelNode *pt, ProgramStack *ps, DataStack *ds, char* command);
#endif //WINDSOLAR_COMMANDS_H

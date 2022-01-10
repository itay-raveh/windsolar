//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_COMMANDS_H
#define WINDSOLAR_COMMANDS_H

#include <stdbool.h>
#include "stack.h"
#include "parse_tree.h"

/* ------------------- Flow Control ------------------- */
bool CALL(LabelNode *pt, Stack *ps, Stack *ds); /* load a subroutine to the ps based on a name in the ds*/

/* -------------------- Arithmetic -------------------- */
bool BINARY_OP(Stack *ds, char *op); /* ADD, SUB, MUL or DIV */

/* ---------------------- Stack ----------------------- */
bool DUP(Stack *ds); /* duplicate the top element */

/* ------------------------ IO ------------------------ */
bool WRITE(Stack *ds); /* pop the top element from the ds and print it */
bool READ(Stack *ds); /* read input from the user and push it to the ds */

/* ---------------------- System ----------------------- */
bool SLEEP(Stack *ds); /* pop the top element and sleep accordingly */

/**
 * Execute a given command. If execution fails, a runtime error will be printed.
 *
 * @param pt - head of the Args_fromArgv tree
 * @param ps - program stack
 * @param ds - data stack
 * @param command - command to execute
 * @return whether execution was successful
 */
bool execCommand(LabelNode *pt, Stack *ps, Stack *ds, char *command);

#endif //WINDSOLAR_COMMANDS_H

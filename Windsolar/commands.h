//
// Created by itay on 1/2/22.
//

#ifndef WINDSOLAR_COMMANDS_H
#define WINDSOLAR_COMMANDS_H

#include <stdbool.h>
#include "stack.h"
#include "parse_tree.h"

/* ------------------- Flow Control ------------------- */
bool CALL(LabelNode *restrict pt, Stack *restrict ps, Stack *restrict ds); /* load a subroutine to the ps based on a name in the ds*/
bool CCALL(LabelNode *restrict pt, Stack *restrict ps, Stack *restrict ds); /* if #2 item is true CALL item #1 */
bool BRANCH(LabelNode *restrict pt, Stack *restrict ps, Stack *restrict ds); /* if #3 item is true, CALL item #2, else item #1 */

/* -------------------- Arithmetic -------------------- */
bool BINARY_OP(Stack *restrict ds, char *restrict op); /* ADD, SUB, MUL, DIV or MOD */

/* ---------------------- Logic ----------------------- */
bool NOT(Stack *restrict ds);
bool BINARY_CMP(Stack *restrict ds, char *restrict cmp); /* EQ, NE, GT, GE, LT, LE, AND, OR */

/* ---------------------- Stack ----------------------- */
bool POP(Stack *restrict ds);
bool DUP(Stack *restrict ds); /* duplicate the top element */
bool DUP2(Stack *restrict ds); /* duplicate the top 2 elements */
bool SWAP12(Stack *restrict ds); /* swap items #1 and #2 */
bool SWAP13(Stack *restrict ds); /* swap items #1 and #3 */

/* ------------------------ IO ------------------------ */
bool WRITE(Stack *restrict ds); /* pop the top element from the ds and print it */
bool READ(Stack *restrict ds); /* read input from the user and push it to the ds */

/* ---------------------- System ----------------------- */
bool SLEEP(Stack *restrict ds); /* pop the top element and sleep accordingly */

/**
 * Execute a given command. If execution fails, a runtime error will be printed.
 *
 * @param pt - head of the Args_fromArgv tree
 * @param ps - program stack
 * @param ds - data stack
 * @param command - command to execute
 * @return whether execution was successful
 */
bool execCommand(LabelNode *restrict pt, Stack *restrict ps, Stack *restrict ds, char *restrict command);

#endif //WINDSOLAR_COMMANDS_H

//
// Created by itay on 12/22/21.
//


/*
 * Parse tree structure
 * -----------------------
 *
 * {LabelNode : 'MAIN'} -> {LabelNode : 'FOO'} -> {LabelNode : 'BAR'}
 *            |                       |                      |
 *            V                       V                      V
 *  {InstNode : '"FOO"'}    {InstNode : '"Hello!"'}         ...
 *            |                       |
 *            V                       V
 *  {InstNode : 'CALL'}              ...
 */

#ifndef WINDSOLAR_PARSE_TREE_H
#define WINDSOLAR_PARSE_TREE_H

#include "token.h"
#include "tokenizer.h"

typedef struct inst_node_t
{
    Token type;                 // Instruction type
    char *str;                  // String og the instruction
    struct inst_node_t *next;   // Next instruction in the block
} InstNode;

InstNode *InstNode_init(Token type, char *str);

void InstNode_free(InstNode *restrict in);


typedef struct label_node_t
{
    char *label;                // The label itself
    struct label_node_t *next;  // Next label in the script
    InstNode *block_head;       // First instruction in the block
} LabelNode;

LabelNode *LabelNode_init(char *label);

void LabelNode_free(LabelNode *ln);

LabelNode *ParseTreeFromTokenizer(Tokenizer *restrict t);

#endif //WINDSOLAR_PARSE_TREE_H

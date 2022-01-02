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

/**
 * Dynamically allocate a new InstNode.
 *
 * @param type - Inst type (i.e. what token is it)
 * @param str - String of the instruction.
 * This should be a dynamically allocated string, which will be freed along with the InstNode.
 * @return ptr to the new InstNode.
 */
InstNode *InstNode_init(Token type, char *str);

/**
 * Free the InstNode and it's str, for the given node and every node following.
 *
 * @param in - InstNode to free.
 */
void InstNode_free(InstNode *restrict in);


typedef struct label_node_t
{
    char *label;                // The label itself
    struct label_node_t *next;  // Next label in the script
    InstNode *block_head;       // First instruction in the block
} LabelNode;

/**
 * Dynamically allocate a new LabelNode.
 *
 * @param label - The label.
 * This should be a dynamically allocated string, which will be freed along with the LabelNode.
 * @return ptr to the new LabelNode.
 */
LabelNode *LabelNode_init(char *label);

/**
 * Free the LabelNode, the label, and the entire list of InstNodes connected to it,
 * for the given node and every node following. i.e. the entire tree will be freed.
 *
 * @param ln - LabelNode to free.
 */
void LabelNode_free(LabelNode *ln);

/**
 * Create a full parse tree based on incoming Tokens from a Tokenizer.
 *
 * @param t - Tokenizer to parse from.
 * @param printTokens - Whether to print Tokens.
 * @return ptr to the head of the tree.
 */
LabelNode *ParseTree_fromTokenizer(Tokenizer *restrict t, bool printTokens);

/**
 * Free entire parse tree.
 *
 * @param head - tree head.
 */
void ParseTree_free(LabelNode *restrict head);

/**
 * Print parse tree.
 *
 * @param head - tree head.
 */
void ParseTree_print(LabelNode *restrict head);

#endif //WINDSOLAR_PARSE_TREE_H

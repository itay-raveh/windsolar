//
// Created by itay on 12/22/21.
//

#include <stdlib.h>     // free()
#include <stdio.h>      // printf(), puts()
#include "parse_tree.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE()


InstNode *InstNode_init(Token type, char *str)
{
    InstNode *in = NEW(InstNode);
    in->type = type;
    in->str = str;
    in->next = NULL;
    return in;
}

void InstNode_free(InstNode *const restrict in)
{
    if (in)
    {
        InstNode_free(in->next);
        free(in->str);
        free(in);
    }
}

LabelNode *LabelNode_init(char *label)
{
    LabelNode *ln = NEW(LabelNode);
    ln->label = label;
    ln->next = NULL;
    ln->block_head = NULL;
    return ln;
}

void LabelNode_free(LabelNode *const restrict ln)
{
    if (ln)
    {
        LabelNode_free(ln->next);
        if (ln->block_head) InstNode_free(ln->block_head);
        free(ln->label);
        free(ln);
    }
}

void ParseTree_free(LabelNode *const restrict head)
{
    TRACE("%s\n", "free ParseTree");
    LabelNode_free(head);
}

void printSyntaxError(Tokenizer *const restrict t, Error e)
{
    printf("SyntaxError: %zu, %lu ('%c'): %s\n", t->lineno, t->charno - t->len, *t->str, error_msg[e]);
}

void printToken(Tokenizer *const restrict t)
{
    char pos[15];
    sprintf(pos, "%zu,%zu-%zu", t->lineno, t->charno - t->len - 1, t->charno - 2);

    char *str = newstr(t->str, t->len);
    printf("%-15s %-15s '%s'\n", pos, token_names[t->token], str);
    free(str);
}

bool nextToken(Tokenizer *const restrict t, bool print)
{
    if (!Tokenizer_next(t))
    {
        printSyntaxError(t, t->err);
        return false;
    }

    if (print) printToken(t);
    return true;
}

LabelNode *ParseTree_fromTokenizer(Tokenizer *const restrict t, bool printTokens)
{
    LabelNode *tree_head = NULL, *last_l, *new_l;

    while (true)
    {
        // Grammar is always the same:
        //
        // LABEL, LPAR, (CMD | STRING | NUMBER, SEMICOL)*, (CMD | STRING | NUMBER)?, RPAR
        //
        // Any deviation should raise an error.

        // NAME (label) or ENDMARKER
        if (!nextToken(t, printTokens))
            return NULL;

        if (t->token == T_ENDMARKER)
            return tree_head;

        if (t->token != T_NAME)
        {
            printSyntaxError(t, E_MISSING_LABEL);
            return NULL;
        }

        new_l = LabelNode_init(newstr(t->str, t->len));

        // LPAR
        if (!nextToken(t, printTokens)) return NULL;

        if (t->token != T_LPAR)
        {
            printSyntaxError(t, E_MISSING_LPAR);
            return NULL;
        }

        // Instructions and RPAR
        InstNode *block_head = NULL, *last_i, *new_i;
        Token last_t = T_LPAR;

        while (true)
        {
            if (!nextToken(t, printTokens)) return NULL;

            switch (t->token)
            {
                case T_LPAR:
                    // currently sub blocks are not supported
                    printSyntaxError(t, E_SUB_BLOCKS_UNSUPPORTED);
                    return NULL;
                case T_RPAR:
                    goto end_loop;
                case T_SEMICOL:
                    last_t = T_SEMICOL;
                    break;
                case T_ENDMARKER:
                    printSyntaxError(t, E_UNCLOSED_BLOCK);
                    return NULL;
                case T_STRING:
                case T_NUMBER:
                case T_NAME: // (command)
                    if (last_t != T_LPAR && last_t != T_SEMICOL)
                    {
                        printSyntaxError(t, E_MISSING_SEMICOL);
                        return NULL;
                    }

                    new_i = InstNode_init(t->token, newstr(t->str, t->len));
                    if (block_head == NULL) block_head = last_i = new_i;
                    else
                    {
                        last_i->next = new_i;
                        last_i = new_i;
                    }

                    last_t = t->token;
                    break;
            }
        }
        end_loop:

        new_l->block_head = block_head;
        if (tree_head == NULL) tree_head = last_l = new_l;
        else
        {
            last_l->next = new_l;
            last_l = new_l;
        }
    }
}

void ParseTree_print(LabelNode *restrict head)
{
    for (; head != NULL; head = head->next)
    {
        printf("{LabelNode | %s}", head->label);

        for (InstNode *block_head = head->block_head; block_head != NULL; block_head = block_head->next)
            printf(" -> {InstNode | %s | %s}", token_names[block_head->type], block_head->str);

        if (head->next) puts("\n           |\n           V");
        else puts("");
    }
}

//
// Created by itay on 12/22/21.
//

#include <stdlib.h>     // free()
#include "parse_tree.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE()
#include <string.h>
#include <stdio.h>


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
    char *str = newstr(t->str, t->len);
    printf("line %2zu, chars %3lu to %3zu:\t\t%-7s\t\t%s\n", t->lineno, t->charno - t->len, t->charno - 1,
           token_names[t->token], str);
    free(str);
}

bool nextToken(Tokenizer *const restrict t, bool verbose)
{
    if (!Tokenizer_next(t))
    {
        printSyntaxError(t, t->err);
        return false;
    }

    if (verbose) printToken(t);
    return true;
}

LabelNode *ParseTree_fromTokenizer(Tokenizer *const restrict t)
{
    LabelNode *tree_head = NULL, *last_l, *new_l;

    while (true)
    {
        // Grammar is always the same:
        //
        // LABEL, LPAR, (CMD | STRING | NUMBER, SEMICOL)*, (CMD | STRING | NUMBER)?, RPAR
        //
        // Any deviation should raise an error.

        // LABEL or ENDMARKER
        if (!nextToken(t, true)) return NULL;

        if (t->token == ENDMARKER) return tree_head;

        if (t->token != LABEL)
        {
            printSyntaxError(t, E_MISSING_LABEL);
            return NULL;
        }

        new_l = LabelNode_init(newstr(t->str, t->len));

        // LPAR
        if (!nextToken(t, true)) return NULL;

        if (t->token != LPAR)
        {
            printSyntaxError(t, E_MISSING_LPAR);
            return NULL;
        }

        // Instructions and RPAR
        InstNode *block_head = NULL, *last_i, *new_i;
        Token last_t = LPAR;

        while (true)
        {
            if (!nextToken(t, true)) return NULL;

            switch (t->token)
            {
                case LPAR:
                case LABEL:
                    // should already be stopped at tokenizer, but still
                    printSyntaxError(t, t->err);
                    return NULL;
                case RPAR:
                    goto end_loop;
                case SEMICOL:
                    last_t = SEMICOL;
                    break;
                case STRING:
                case NUMBER:
                case CMD:
                    if (last_t != LPAR && last_t != SEMICOL)
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
                case ENDMARKER:
                    printSyntaxError(t, E_UNCLOSED_BLOCK);
                    return NULL;
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
        if (head->next) puts("\n           |\n           V"); else puts("");
    }
}

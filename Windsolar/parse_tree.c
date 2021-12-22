//
// Created by itay on 12/22/21.
//

#include <stdlib.h>     // free()
#include "parse_tree.h"
#include "utils.h"      // NEW()
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
    if (in) InstNode_free(in->next);
    free(in->str);
    free(in);
}

LabelNode *LabelNode_init(char *label)
{
    LabelNode *ln = NEW(LabelNode);
    ln->label = label;
    ln->next = NULL;
    ln->block_head = NULL;
    return ln;
}

void LabelNode_free(LabelNode *ln)
{
    if (ln) LabelNode_free(ln->next);
    if (ln->block_head) InstNode_free(ln->block_head);
    free(ln->label);
    free(ln);
}

LabelNode *ParseTreeFromTokenizer(Tokenizer *restrict t)
{
    while (Tokenizer_next(t))
    {
        char *str = (char *) malloc_s(t->len + 1);
        if (t->len > 0)
            strncpy(str, t->str, t->len);
        str[t->len] = '\0';

        printf("%2zu,%3lu-%3lu:\t\t%-10s\t%s\n", t->lineno, t->charno - t->len, t->charno - 1, token_names[t->token],
               str);

        free(str);

        if (t->token == ENDMARKER) break;
    }

    if (t->err)
    {
        printf("Syntax Error: %zu, %lu ('%c'): %s\n", t->lineno, t->charno - t->len, *t->str, error_msg[t->err]);
        return NULL;
    }

    return NULL;
}

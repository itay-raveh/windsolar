//
// Created by itay on 11/29/21.
//

#include <stdlib.h>
#include <assert.h>
#include "token.h"
#include "utils.h"

int Token_isContentChr(Token *t)
{
    assert(t);

    switch (t->type)
    {
        case LPAR:
        case RPAR:
        case SEMICOL:
            return 1;
        default:
            return 0;
    }
}

int Token_isContentStr(Token *t)
{
    assert(t);

    switch (t->type)
    {
        case STR_LIT:
        case NAME:
            return 1;
        default:
            return 0;
    }
}

int Token_isContentNum(Token *t)
{
    assert(t);

    switch (t->type)
    {
        case NUM_LIT:
            return 1;
        default:
            return 0;
    }
}

int Token_isContentAction(Token *t)
{
    assert(t);

    switch (t->type)
    {
        case ACTION:
            return 1;
        default:
            return 0;
    }
}

Token *Token_init(token_type_t type, token_content_t content)
{
    Token *t = MALLOC(Token);
    t->type = type;
    t->content = content;
    return t;
}

void Token_free(Token *t)
{
    assert(t);

    if (Token_isContentStr(t)) free(t->content.str);
    free(t);
}

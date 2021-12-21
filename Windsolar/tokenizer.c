//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <stdlib.h>     // free()
#include <ctype.h>      // isspace(), isalpha(), isdigit()
#include "tokenizer.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE()

Tokenizer *Tokenizer_init(FileReader *const restrict fr)
{
    assert(fr);
    TRACE("%s", "init Tokenizer\n");

    Tokenizer *t = NEW(Tokenizer);
    t->fr = fr;
    t->in_block = 0;
    t->token = -1;
    t->str = NULL;
    t->len = 0;
    t->err = OK;
    t->lineno = 0;
    t->charno = 0;
    return t;
}

inline void Tokenizer_free(Tokenizer *const restrict t)
{
    assert(t);
    TRACE("%s", "free Tokenizer\n");

    FileReader_free(t->fr);
    free(t);
}

bool islabel(const char c)
{
    return (c == '_' || c == '-' || isalpha(c));
}

bool Tokenizer_next(Tokenizer *const restrict t)
{
    #define NEXTC (FileReader_next(t->fr))
    #define CURRC (FileReader_curr(t->fr))
    assert(t);

    char c;

    skip_spaces:
    do c = *(t->fr->curr == 0 ? CURRC : NEXTC);
    while (isspace(c));

    t->lineno = t->fr->lineno;
    t->charno = t->fr->charno;

    // most common values
    t->str = CURRC;
    t->len = 1;

    // skip comments
    if (c == '#')
    {
        do c = *NEXTC;
        while (c != '#' && c != '\0');

        if (c == '\0')
        {
            t->err = UNCLOSED_COMMENT;
            return false;
        } else goto skip_spaces;
    }

    // ENDMARKER
    if (c == '\0')
    {
        t->str = NULL;
        t->len = 0;
        t->token = ENDMARKER;
        return true;
    }

    if (!t->in_block)
    {
        // LPAR
        if (c == '(')
        {
            t->in_block = true;
            t->token = LPAR;
            return true;
        }

        // LABEL
        if (islabel(c))
        {
            t->len = 0;
            while (islabel(*CURRC))
            {
                NEXTC;
                t->len++;
            }
            t->lineno = t->fr->lineno - (*CURRC == '\n' ? 1 : 0);
            t->charno = t->fr->charno - 1;
            t->token = LABEL;
            return true;
        }

        t->err = UNEXPECTED_CHAR;
        return false;
    }

    // RPAR
    if (c == ')')
    {
        t->in_block = false;
        t->token = RPAR;
        return true;
    }

    // SEMICOL
    if (c == ';')
    {
        t->token = SEMICOL;
        return true;
    }

    // NUMBER
    if (isdigit(c))
    {
        t->len = 0;
        while (isdigit(*CURRC))
        {
            NEXTC;
            t->len++;
        }
        t->lineno = t->fr->lineno - (*CURRC == '\n' ? 1 : 0);
        t->charno = t->fr->charno - 1;
        t->token = NUMBER;
        return true;
    }

    // STRING
    if (c == '"')
    {
        do
        {
            c = *NEXTC;
            t->len++;
        } while (c != '"' && c != '\0');
        NEXTC;

        if (c == '\0')
        {
            t->err = UNCLOSED_STRING;
            return false;
        }
        t->lineno = t->fr->lineno - (*CURRC == '\n' ? 1 : 0);
        t->charno = t->fr->charno - 1;
        t->token = STRING;
        return true;
    }

    // CMD
    if (isalpha(c))
    {
        t->len = 0;
        while (isalnum(*CURRC))
        {
            NEXTC;
            t->len++;
        }
        t->lineno = t->fr->lineno - (*CURRC == '\n' ? 1 : 0);
        t->charno = t->fr->charno - 1;
        t->token = CMD;
        return true;
    }

    t->err = UNEXPECTED_CHAR;
    return false;
}

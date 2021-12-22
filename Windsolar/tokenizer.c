//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <stdlib.h>     // free()
#include <ctype.h>      // isspace(), isalpha(), isdigit()
#include "tokenizer.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE()

Tokenizer *Tokenizer_init(Reader *const restrict r)
{
    assert(r);
    TRACE("%s", "init Tokenizer\n");

    Tokenizer *t = NEW(Tokenizer);
    t->reader = r;
    t->in_block = 0;
    t->token = -1;
    t->str = NULL;
    t->len = 0;
    t->err = E_OK;
    t->lineno = 0;
    t->charno = 0;
    return t;
}

inline void Tokenizer_free(Tokenizer *const restrict t)
{
    assert(t);
    TRACE("%s", "free Tokenizer\n");

    Reader_free(t->reader);
    free(t);
}

bool islabel(const char c)
{
    return (c == '_' || c == '-' || isalpha(c));
}

bool Tokenizer_next(Tokenizer *const restrict t)
{
    #define NEXTC (Reader_next(t->reader))
    #define CURRC (Reader_curr(t->reader))
    assert(t);

    char c;

    skip_spaces:
    do c = *(t->reader->curr == 0 ? CURRC : NEXTC);
    while (isspace(c));

    t->lineno = t->reader->lineno;
    t->charno = t->reader->charno;

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
            t->err = E_UNCLOSED_COMMENT;
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
            t->lineno = t->reader->lineno - (*CURRC == '\n' ? 1 : 0);
            t->charno = t->reader->charno - 1;
            t->token = LABEL;
            return true;
        }

        t->err = E_UNEXPECTED_CHAR;
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
        t->lineno = t->reader->lineno - (*CURRC == '\n' ? 1 : 0);
        t->charno = t->reader->charno - 1;
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
            t->err = E_UNCLOSED_STRING;
            return false;
        }
        t->lineno = t->reader->lineno - (*CURRC == '\n' ? 1 : 0);
        t->charno = t->reader->charno - 1;
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
        t->lineno = t->reader->lineno - (*CURRC == '\n' ? 1 : 0);
        t->charno = t->reader->charno - 1;
        t->token = CMD;
        return true;
    }

    t->err = E_UNEXPECTED_CHAR;
    return false;
}

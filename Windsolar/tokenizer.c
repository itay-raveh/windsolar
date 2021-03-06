//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <stdlib.h>     // free()
#include <ctype.h>      // isspace(), isalpha(), isdigit()
#include "tokenizer.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE()

Tokenizer *Tokenizer_fromReader(Reader *const restrict r)
{
    assert(r);

    Tokenizer *t = NEW(Tokenizer);
    t->reader = r;
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

    free(t);
}

bool isSingleCharToken(const char c)
{
    return (c == '(' || c == ')' || c == ';' || c == '\0');
}

bool isLegalNameChar(const char c)
{
    return (c != '#' && c != '"' && !isspace(c) && !isSingleCharToken(c));
}

bool Tokenizer_next(Tokenizer *const restrict t)
{
    #define NEXTC (Reader_next(t->reader))
    #define CURRC (Reader_curr(t->reader))
    assert(t);

    skip_spaces:
    if (isspace(*CURRC))
        while (isspace(*NEXTC));

    t->str = CURRC;
    char c = *(t->str);

    // skip comment
    if (c == '#')
    {
        do c = *NEXTC;
        while (c != '#' && c != '\0');

        if (c == '\0')
        {
            t->err = E_UNCLOSED_COMMENT;
            return false;
        } else
        {
            NEXTC;
            goto skip_spaces;
        }

    }

    if (isSingleCharToken(c))
    {

        t->lineno = t->reader->lineno;
        t->charno = t->reader->charno + 1;
        t->len = 1;

        switch (c)
        {
            case ';':t->token = T_SEMICOL;
                break;
            case '(':t->token = T_LPAR;
                break;
            case ')':t->token = T_RPAR;
                break;
            case '\0':t->token = T_ENDMARKER;
                return true;
            default:assert(false); // Should be impossible
        }

        NEXTC;
        return true;
    }

    t->len = 0;

    if (c == '"')
    {
        t->token = T_STRING;

        t->str = NEXTC;
        do
        {
            c = *NEXTC;
            t->len++;
        } while (c != '"' && c != '\0');

        if (c == '\0')
        {
            t->err = E_UNCLOSED_STRING;
            return false;
        }

        // second "
        NEXTC;
    } else if (isdigit(c) || c == '-')
    {
        t->token = T_NUMBER;

        do
        {
            c = *NEXTC;
            t->len++;
        } while (isdigit(c));

        // if the decimal point is right after the minus
        if (*(t->str) == '-' && t->len == 1)
        {
            t->err = E_UNPARSABLE_NUMBER;
            return false;
        }

        if (c == '.')
        {
            size_t len_before = t->len;
            do
            {
                c = *NEXTC;
                t->len++;
            } while (isdigit(c));

            // if there were no digits after the point
            if (t->len == len_before + 1)
            {
                t->err = E_UNPARSABLE_NUMBER;
                return false;
            }
        }
    } else
    {
        t->token = T_NAME;

        while (isLegalNameChar(c))
        {
            c = *NEXTC;
            t->len++;
        }
    }

    t->lineno = t->reader->lineno;
    t->charno = t->reader->charno - (t->token == T_STRING ? 1 : 0);
    return true;
}

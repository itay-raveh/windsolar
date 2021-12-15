//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <string.h>     // strlen()
#include <stdlib.h>     // free()
#include <ctype.h>      // isspace(), isalpha(), isdigit()
#include "tokenizer.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE()

Tokenizer *Tokenizer_init(FileReader *fr)
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
    return t;
}

void Tokenizer_free(Tokenizer *t)
{
    assert(t);
    TRACE("%s", "free Tokenizer\n");

    FileReader_free(t->fr);
    free(t);
}

bool islabel(char c)
{
    return (c == '_' || c == '-' || isalpha(c));
}

bool Tokenizer_next(Tokenizer *t)
{
    assert(t);

    char c;

    skip_spaces:
    do
    {
        c = FileReader_next(t->fr);
    } while (isspace(c));

    // skip comments
    if (c == '#')
    {
        do
        {
            c = FileReader_next(t->fr);
        } while (c != '#' && c != EOF);

        if (c == EOF)
            t->err = UNCLOSED_COMMENT;
            return false;
        } else goto skip_spaces;
    }

    // check for EOF
    if (c == EOF)
    {
        t->token = ENDMARKER;
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
            t->str = &(t->fr->buff[t->fr->curr]);

            int32_t len = 1;
            while (islabel(FileReader_next(t->fr))) len++;
            FileReader_back(t->fr);

            t->len = len;
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
        t->str = &(t->fr->buff[t->fr->curr]);
        t->len = 1;
        t->token = SEMICOL;
        return true;
    }

    // NUMBER
    if (isdigit(c))
    {
        t->str = &(t->fr->buff[t->fr->curr]);

        int32_t len = 1;
        while (isdigit(FileReader_next(t->fr))) len++;
        FileReader_back(t->fr);

        t->len = len;
        t->token = NUMBER;
        return true;
    }

    // STRING
    if (c == '"')
    {
        t->str = &(t->fr->buff[t->fr->curr]);

        int32_t len = 0;
        while ('"' != FileReader_next(t->fr)) len++;
        FileReader_back(t->fr);

        t->len = len;
        t->token = STRING;
        return true;
    }

    // CMD
    if (isalpha(c))
    {
        t->str = &(t->fr->buff[t->fr->curr]);

        int32_t len = 1;
        while (isalnum(FileReader_next(t->fr))) len++;
        FileReader_back(t->fr);

        t->len = len;
        t->token = CMD;
        return true;
    }

    t->err = UNEXPECTED_CHAR;
    return false;
}

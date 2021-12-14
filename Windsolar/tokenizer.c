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

int32_t islabel(char c)
{
    return (c == '_' || c == '-' || isalpha(c));
}

int32_t Tokenizer_next(Tokenizer *t)
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
        else
            goto skip_spaces;
    }

    // check for EOF
    if (c == EOF)
    {
        t->token = ENDMARKER;
        t->str = NULL;
        t->len = 0;
        return 1;
    }

    if (!t->in_block)
    {
        // LPAR
        if (c == '(')
        {
            t->in_block = 1;
            t->token = LPAR;
            t->str = &(t->fr->buff[t->fr->curr]);
            t->len = 1;
            return 1;
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
            return 1;
        }

        t->err = UNEXPECTED_CHAR;
        return 0;
    }

    // RPAR
    if (c == ')')
    {
        t->in_block = 0;
        t->token = RPAR;
        t->str = &(t->fr->buff[t->fr->curr ]);
        t->len = 1;
        return 1;
    }

    // SEMICOL
    if (c == ';')
    {
        t->str = &(t->fr->buff[t->fr->curr]);
        t->len = 1;
        t->token = SEMICOL;
        return 1;
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
        return 1;
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
        return 1;
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
        return 1;
    }

    t->err = UNEXPECTED_CHAR;
    return 0;
}

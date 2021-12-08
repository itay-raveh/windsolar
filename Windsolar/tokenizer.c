//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <string.h>     // strlen()
#include <stdlib.h>     // free()
#include <ctype.h>      // isspace(), isalpha()
#include "tokenizer.h"
#include "utils.h"      // MALLOC()
#include "macros.h"     // TRACE(), EXIT_WITH_MSG()

#define MAX_STR_LEN 1000

Tokenizer *Tokenizer_init(FileReader *fr)
{
    assert(fr);
    TRACE("%s", "init Tokenizer\n");

    Tokenizer *t = MALLOC(Tokenizer);
    t->fr = fr;
    t->in_block = 0;
    t->lineno = 1;
    return t;
}

void Tokenizer_free(Tokenizer *t)
{
    assert(t);
    TRACE("%s", "free Tokenizer\n");

    FileReader_free(t->fr);
    free(t);
}

int islabel(char c)
{
    return (c == '_' || c == '-' || isalpha(c));
}


Token Tokenizer_next(Tokenizer *t, char **p_start, char **p_end)
{
    assert(t);
    assert(!FileReader_isEOF(t->fr));

    if(FileReader_isEOF(t->fr)) return EOF;

    *p_start = *p_end = NULL;

    // skip leading spaces
    while (isspace(FileReader_peek(t->fr, 0)))
    {
        if ('\n' == FileReader_peek(t->fr, 0))
            t->lineno++;
        FileReader_consume(t->fr, 0);
        if(FileReader_isEOF(t->fr)) return EOF;
    }

    if (!t->in_block)
    {
        // LPAR
        if ('(' == FileReader_peek(t->fr, 0))
        {
            *p_start = &(t->fr->buff[t->fr->i]);
            *p_end = *p_start + 1;
            FileReader_consume(t->fr, 0);
            t->in_block = 1;
            return LPAR;
        }

        // LABEL
        if (islabel(FileReader_peek(t->fr, 0)))
        {

            *p_start = &(t->fr->buff[t->fr->i]);

            int32_t len = 0;
            while (islabel(FileReader_consume(t->fr, 0))) len++;

            if (len >= MAX_STR_LEN)
                EXIT_WITH_MSG(EXIT_FAILURE, "Error: labels or strings longer than %d chars not allowed\n", MAX_STR_LEN);

            *p_end = &(t->fr->buff[t->fr->i]);
            return LABEL;
        }

        EXIT_WITH_MSG(EXIT_FAILURE, "Error: only labels are allowed outside of blocks (line %d)\n", t->lineno);
    }

    // RPAR
    if (')' == FileReader_peek(t->fr, 0))
    {
        *p_start = &(t->fr->buff[t->fr->i]);
        *p_end = *p_start + 1;
        FileReader_consume(t->fr, 0);
        t->in_block = 0;
        return RPAR;
    }

    // SEMICOL
    if (';' == FileReader_peek(t->fr, 0))
    {
        *p_start = &(t->fr->buff[t->fr->i]);
        *p_end = *p_start + 1;
        FileReader_consume(t->fr, 0);
        return SEMICOL;
    }

    // NUMBER
    if (isdigit(FileReader_peek(t->fr, 0)))
    {
        *p_start = &(t->fr->buff[t->fr->i]);

        while (isdigit(FileReader_peek(t->fr, 0))) FileReader_consume(t->fr, 0);

        *p_end = &(t->fr->buff[t->fr->i]);
        return NUMBER;
    }

    // STRING
    if ('"' == FileReader_peek(t->fr, 0))
    {
        *p_start = &(t->fr->buff[t->fr->i]);

        FileReader_consume(t->fr, 0);
        while ('"' != FileReader_consume(t->fr, 0));

        *p_end = &(t->fr->buff[t->fr->i]);
        return STRING;
    }

    // CMD
    if (isalpha(FileReader_peek(t->fr, 0)))
    {
        *p_start = &(t->fr->buff[t->fr->i]);

        while (isalpha(FileReader_peek(t->fr, 0))) FileReader_consume(t->fr, 0);

        *p_end = &(t->fr->buff[t->fr->i]);
        return CMD;
    }

    EXIT_WITH_MSG(EXIT_FAILURE, "Error: unexpected char '%c' in program block (line %d)\n", FileReader_peek(t->fr, 0),
                  t->lineno);
}

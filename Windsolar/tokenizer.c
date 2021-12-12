//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <string.h>     // strlen()
#include <stdlib.h>     // free()
#include <ctype.h>      // isspace(), isalpha(), isdigit()
#include "tokenizer.h"
#include "utils.h"      // NEW()
#include "macros.h"     // TRACE(), EXIT_WITH_MSG()

#define MAX_STR_LEN 1000

Tokenizer *Tokenizer_init(FileReader *fr)
{
    assert(fr);
    TRACE("%s", "init Tokenizer\n");

    Tokenizer *t = NEW(Tokenizer);
    t->fr = fr;
    t->in_block = 0;
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


Token Tokenizer_next(Tokenizer *t, char **str, size_t *len)
{
    assert(t);

    char c;

    skip_spaces:
    do
    {
        c = FileReader_consume(t->fr, 0);
    } while (isspace(c));

    // skip comments
    if (c == '#')
    {
        do
        {
            c = FileReader_consume(t->fr, 0);
        } while (c != '#' && c != '\0');
        if (c != '\0') goto skip_spaces;
    }

    // check for EOF
    if (c == '\0') return EOF;

    if (!t->in_block)
    {
        // LPAR
        if (c == '(')
        {
            t->in_block = 1;
            *str = &(t->fr->buff[t->fr->curr - 1]);
            *len = 1;
            return LPAR;
        }

        // LABEL
        if (islabel(c))
        {
            *str = &(t->fr->buff[t->fr->curr - 1]);

            *len = 1;
            while (islabel(FileReader_consume(t->fr, 0))) (*len)++;

            if (*len >= MAX_STR_LEN)
                EXIT_WITH_MSG(EXIT_FAILURE, "Error: labels or strings longer than %d chars not allowed\n", MAX_STR_LEN);

            return LABEL;
        }

        EXIT_WITH_MSG(EXIT_FAILURE, "Error: %d, %d: only labels (comprised only of alphabetic chars, '-'s and '_'s)"
                                    " are allowed outside of blocks\n", t->fr->lineno, t->fr->charno);
    }

    // RPAR
    if (c == ')')
    {
        t->in_block = 0;

        *str = &(t->fr->buff[t->fr->curr - 1]);
        *len = 1;
        return RPAR;
    }

    // SEMICOL
    if (c == ';')
    {
        *str = &(t->fr->buff[t->fr->curr - 1]);
        *len = 1;
        return SEMICOL;
    }

    // NUMBER
    if (isdigit(c))
    {
        *str = &(t->fr->buff[t->fr->curr - 1]);

        *len = 1;
        while (isdigit(FileReader_consume(t->fr, 0)))
            (*len)++;

        return NUMBER;
    }

    // STRING
    if (c == '"')
    {
        *str = &(t->fr->buff[t->fr->curr]);
        *len = 0;
        while ('"' != FileReader_consume(t->fr, 0)) (*len)++;

        return STRING;
    }

    // CMD
    if (isalpha(c))
    {
        *str = &(t->fr->buff[t->fr->curr - 1]);
        *len = 1;
        while (isalpha(FileReader_consume(t->fr, 0))) (*len)++;

        return CMD;
    }

    EXIT_WITH_MSG(EXIT_FAILURE, "Error: %d, %d: unexpected char '%c' in program block\n", t->fr->lineno, t->fr->charno,
                  FileReader_peek(t->fr, 0));
}

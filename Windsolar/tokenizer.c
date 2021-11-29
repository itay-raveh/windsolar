//
// Created by itay on 11/29/21.
//

#include <assert.h>     // assert()
#include <string.h>     // strlen()
#include <stdlib.h>     // free()
#include "tokenizer.h"
#include "utils.h"      // MALLOC()

Tokenizer *Tokenizer_init(char *fname, size_t fsize)
{
    assert(fname);
    assert(strlen(fname) >= 1);
    assert(fsize >= 0);

    Tokenizer *t = MALLOC(Tokenizer);
    t->fr = FileReader_init(fname, fsize);
    return t;
}

void Tokenizer_free(Tokenizer *t)
{
    assert(t);

    FileReader_free(t->fr);
    free(t);
}

Token *Tokenizer_next(Tokenizer *t)
{
    return NULL;
}

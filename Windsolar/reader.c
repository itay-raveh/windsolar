//
// Created by itay on 11/22/21.
//

#include <string.h>         // strlen()
#include <assert.h>         // assert()
#include "reader.h"
#include "utils.h"          // NEW()
#include "macros.h"         // EXIT_WITH_MSG()

Reader *Reader_fromFile(const char *restrict fname, size_t size)
{
    assert(fname);
    assert(strlen(fname) >= 1);
    assert(size >= 0);
    TRACE("init Reader with fname='%s' size=%lu\n", fname, size);

    Reader *fr = NEW(Reader);
    fr->buff = malloc_s(size + 1); // +1 for \0
    fr->size = size;
    fr->curr = 0;
    fr->lineno = 1;
    fr->charno = 1;

    FILE *f = fopen(fname, "r");
    if (f == NULL)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not open %s\n", fname);

    size_t read_bytes = fread(fr->buff, sizeof(char), size, f);

    int err = ferror(f);
    fclose(f);

    if (err != 0 || read_bytes != size)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not read from %s\n", fname);

    fr->buff[size] = '\0';
    return fr;
}

inline void Reader_free(Reader *const restrict r)
{
    assert(r);
    TRACE("%s", "free Reader\n");

    free(r->buff);
    free(r);
}

inline char *Reader_curr(const Reader *const restrict r)
{
    assert(r);

    return r->curr > r->size ? NULL : &(r->buff[r->curr]);
}

char *Reader_next(Reader *const restrict r)
{
    assert(r);
    assert(r->curr < 0 || r->curr <= r->size);

    if (r->curr >= r->size) return NULL;

    r->curr++;
    char *c = &(r->buff[r->curr]);

    if (*c == '\n')
    {
        r->lineno++;
        r->charno = 0;
    }
    r->charno++;

    return c;
}

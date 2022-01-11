//
// Created by itay on 11/22/21.
//

#include <assert.h>         // assert()
#include "reader.h"
#include "utils.h"          // NEW()
#include "macros.h"         // EXIT_WITH_MSG()


Reader *Reader_fromFile(const char *restrict fname)
{
    assert(fname);
    assert(strlen(fname) >= 1);

    Reader *r = NEW(Reader);
    r->curr = 0;
    r->lineno = 1;
    r->charno = 1;

    FILE *f = fopen(fname, "r");
    if (f == NULL) return NULL;

    /* find file size */
    if (fseek(f, 0, SEEK_END) != 0) goto error;
    r->size = ftell(f);
    if (r->size == -1) goto error;
    if (fseek(f, 0, SEEK_SET) != 0) goto error;

    r->buff = malloc_s(sizeof(char) * (r->size + 1)); // +1 for \0

    /* read file into buffer */
    size_t read_bytes = fread(r->buff, sizeof(char), r->size, f);
    if (ferror(f) != 0) goto error;
    r->buff[read_bytes] = '\0';

    /* since lineno is advanced in the Reader_next method,
     * a newline at the start of the file will not be counted */
    if (*(r->buff) == '\n')
        r->lineno++;

    fclose(f);
    return r;

    error:
    fclose(f);
    return NULL;
}

void Reader_free(Reader *const restrict r)
{
    assert(r);

    free(r->buff);
    free(r);
}

char *Reader_curr(const Reader *const restrict r)
{
    assert(r);

    return r->curr > r->size ? NULL : &(r->buff[r->curr]);
}

char *Reader_next(Reader *const restrict r)
{
    assert(r);
    assert(r->curr < 0 || r->curr <= r->size);

    if (r->curr > r->size) return NULL;

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

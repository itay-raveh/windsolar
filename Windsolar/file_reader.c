//
// Created by itay on 11/22/21.
//

#include <string.h>         // strlen()
#include <assert.h>         // assert()
#include "file_reader.h"
#include "utils.h"          // NEW()
#include "macros.h"         // EXIT_WITH_MSG()

FileReader *FileReader_init(char *fname, size_t fsize)
{
    assert(fname);
    assert(strlen(fname) >= 1);
    assert(fsize >= 0);
    TRACE("init FileReader with fname='%s' fsize=%lu\n", fname, fsize);

    FileReader *fr = NEW(FileReader);
    fr->buff = malloc_s(fsize + 1); // +1 for \0
    fr->fsize = fsize;
    fr->curr = -1;
    fr->lineno = 1;
    fr->charno = 1;
    fr->last_line_len = 0;

    FILE *f = fopen(fname, "r");
    if (f == NULL)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not open %s\n", fname);

    size_t read_bytes = fread(fr->buff, sizeof(char), fsize, f);
    fclose(f);

    if (ferror(f) != 0 || read_bytes != fsize)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not read from %s\n", fname);

    fr->buff[fsize] = '\0';
    return fr;
}

void FileReader_free(FileReader *fr)
{
    assert(fr);
    TRACE("%s", "free FileReader\n");

    free(fr->buff);
    free(fr);
}

char FileReader_next(FileReader *fr)
{
    assert(fr);
    assert(fr->curr < 0 || fr->curr <= fr->fsize);

    if (fr->curr == fr->fsize) return EOF;

    fr->curr++;
    char c = fr->buff[fr->curr];
    if (c == '\0') return EOF;

    if (c == '\n')
    {
        fr->lineno++;
        fr->last_line_len = fr->charno;
        fr->charno = 0;
    }

    fr->charno++;
    return c;
}

char FileReader_back(FileReader *fr)
{
    assert(fr);
    assert(fr->curr > 0);

    if (fr->curr == 0) return fr->buff[fr->curr];

    fr->curr--;
    char c = fr->buff[fr->curr];

    if (c == '\n')
    {
        fr->lineno--;
        fr->charno = fr->last_line_len;
    }

    fr->charno--;
    return c;
}

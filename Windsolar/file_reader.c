//
// Created by itay on 11/22/21.
//

#include <string.h>         // strlen()
#include <assert.h>         // assert()
#include "file_reader.h"
#include "utils.h"          // MALLOC()
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
    fr->curr = 0;
    fr->lineno = 1;
    fr->charno = 1;

    FILE *f = fopen(fname, "r");
    if (f == NULL)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not open %s\n", fname);

    size_t read_bytes = fread(fr->buff, sizeof(char), fsize, f);
    if (ferror(f) != 0 | read_bytes != fsize)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not read from %s\n", fname);

    fr->buff[fsize] = '\0';
    fclose(f);

    return fr;
}

void FileReader_free(FileReader *fr)
{
    assert(fr);
    TRACE("%s", "free FileReader\n");

    free(fr->buff);
    free(fr);
}

char FileReader_peek(FileReader *fr, int32_t k)
{
    assert(fr);
    assert(k >= 0);
    assert(fr->curr + k < fr->fsize);

    return fr->buff[fr->curr + k];
}

char FileReader_consume(FileReader *fr, int32_t k)
{
    char c = FileReader_peek(fr, k);
    fr->curr += k + 1;
    if (c == '\n')
    {
        fr->lineno += 1;
        fr->charno = 1;
    } else fr->charno += k + 1;
    return c;
}

int32_t FileReader_isEOF(FileReader *fr)
{
    assert(fr);

    return fr->curr == fr->fsize;
}

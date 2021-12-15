//
// Created by itay on 11/22/21.
//

#include <string.h>         // strlen()
#include <assert.h>         // assert()
#include "file_reader.h"
#include "utils.h"          // NEW()
#include "macros.h"         // EXIT_WITH_MSG()

FileReader *FileReader_init(const char *const restrict fname, const size_t fsize)
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

    int err = ferror(f);
    fclose(f);

    if (err != 0 || read_bytes != fsize)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not read from %s\n", fname);

    fr->buff[fsize] = '\0';
    return fr;
}

inline void FileReader_free(FileReader *const restrict fr)
{
    assert(fr);
    TRACE("%s", "free FileReader\n");

    free(fr->buff);
    free(fr);
}

inline char *FileReader_curr(const FileReader *const restrict fr)
{
    assert(fr);

    return fr->curr > fr->fsize ? NULL : &(fr->buff[fr->curr]);
}

char *FileReader_next(FileReader *const restrict fr)
{
    assert(fr);
    assert(fr->curr < 0 || fr->curr <= fr->fsize);

    if (fr->curr >= fr->fsize) return NULL;

    fr->curr++;
    char *c = &(fr->buff[fr->curr]);

    if (*c == '\n')
    {
        fr->lineno++;
        fr->charno = 0;
    }
    fr->charno++;

    return c;
}

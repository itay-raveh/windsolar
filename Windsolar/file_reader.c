//
// Created by itay on 11/22/21.
//
#include <stdlib.h>
#include <assert.h>
#include "file_reader.h"
#include "utils.h"
#include "macros.h"

FileReader *FileReader_init(char *fname, size_t fsize)
{
    assert(fname);
    assert(fsize >= 0);

    FileReader *fr = (FileReader *) malloc_s(sizeof(FileReader));
    fr->buff = malloc_s(fsize + 1); // +1 for \0
    fr->fsize = fsize;
    fr->i = 0;

    FILE *f = fopen(fname, "r");
    if (f == NULL)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not open %s\n", fname);

    int16_t read_bytes = fread(fr->buff, sizeof(char), fsize, f);
    if (ferror(f) != 0 | read_bytes != fsize)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: could not read from %s\n", fname);

    fr->buff[fsize] = '\0';
    fclose(f);

    return fr;
}

void FileReader_free(FileReader *fr)
{
    assert(fr);

    free(fr->buff);
    free(fr);
}

char FileReader_peek(FileReader *fr, int k)
{
    assert(fr);
    assert(k >= 0);
    assert(fr->i + k < fr->fsize);

    return fr->buff[fr->i + k];
}

char FileReader_consume(FileReader* fr, int k)
{
    char c = FileReader_peek(fr, k);
    fr->i += k + 1;
    return c;
}

//
// Created by itay on 11/22/21.
//

#ifndef WINDSOLAR_READER_H
#define WINDSOLAR_READER_H

#include <stddef.h>     // size_t
#include <inttypes.h>   // int32_t

typedef struct
{
    char *buff;     // Holds the entire input
    size_t size;    // The size of the input
    int32_t curr;   // Current char index, relative to file start
    size_t charno;  // Current char index, relative to start of current line
    size_t lineno;  // Current Line number
} Reader;

/**
 * Dynamically allocate a new Reader and init it from a file
 *
 * @param fname - name of a file that can be opened for reading
 * @param fsize - size of the file in bytes
 * @return ptr to the new Reader
 */
Reader *Reader_fromFile(const char *restrict fname, size_t fsize);

/**
 * Free a dynamically allocated Reader.
 *
 * @param r - Reader to free
 */
void Reader_free(Reader *restrict r);

/**
 * Get ptr to current char
 * @param r - Reader
 * @return ptr to char from the buffer or NULL
 */
char *Reader_curr(const Reader *restrict r);

/**
 * Move one char up the buffer and return it.
 *
 * @param r - Reader
 * @return ptr to char from the buffer or NULL
 */
char *Reader_next(Reader *restrict r);

#endif //WINDSOLAR_READER_H

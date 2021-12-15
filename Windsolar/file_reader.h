//
// Created by itay on 11/22/21.
//

#ifndef WINDSOLAR_FILE_READER_H
#define WINDSOLAR_FILE_READER_H

#include <stddef.h>     // size_t
#include <inttypes.h>   // int32_t

typedef struct
{
    char *buff;     // Holds the entire file
    size_t fsize;   // The size of the file
    int32_t curr;   // Current char index, relative to file start
    size_t charno;  // Current char index, relative to start of current line
    size_t lineno;  // Current Line number
} FileReader;

/**
 * Dynamically allocate a new FileReader and read the file into the buffer.
 *
 * @param fname - name of a file that can be opened for reading
 * @param fsize - size of the file in bytes
 * @return ptr to the new FileReader
 */
FileReader *FileReader_init(char *fname, size_t fsize);

/**
 * Free a dynamically allocated FileReader.
 *
 * @param fr - FileReader to free
 */
void FileReader_free(FileReader *fr);

/**
 * Get ptr to current char
 * @param fr - FileReader
 * @return ptr to char from the buffer or NULL
 */
char *FileReader_curr(FileReader *fr);

/**
 * Move one char up the buffer and return it.
 *
 * @param fr - FileReader
 * @return ptr to char from the buffer or NULL
 */
char *FileReader_next(FileReader *fr);

#endif //WINDSOLAR_FILE_READER_H

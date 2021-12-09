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
    int32_t charno; // Current char index, relative to start of current line
    int32_t lineno; // Line number
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
 * Get the char k places after the current char.
 *
 * @param fr
 * @param k
 * @return The k-th char
 */
char FileReader_peek(FileReader *fr, int32_t k);

/**
 * Get the char k places after current char and move the index to that place.
 *
 * @param fr
 * @param k
 * @return The k-th
 */
char FileReader_consume(FileReader *fr, int32_t k);

/**
 * Checks if fr is at EOF
 * @param fr
 * @return 1 if EOF, else 0
 */
int32_t FileReader_isEOF(FileReader *fr);

#endif //WINDSOLAR_FILE_READER_H

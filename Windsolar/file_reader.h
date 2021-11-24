//
// Created by itay on 11/22/21.
//

#ifndef WINDSOLAR_FILE_READER_H
#define WINDSOLAR_FILE_READER_H

#include <stdio.h>

typedef struct file_reader_t
{
    char *buff;     // Holds the entire file.
    int i;          // Index of the last consumed char
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
void FileReader_free(FileReader* fr);

/**
 * Get the char k places after the fr->i.
 *
 * @param fr
 * @param k
 * @return The k-th char
 */
char FileReader_peek(FileReader* fr, int k);

/**
 * Get the char k places after fr->i and move the index to that place.
 *
 * @param fr
 * @param k
 * @return The k-th
 */
char FileReader_consume(FileReader* fr, int k);

/**
 * Checks if fr->i is at EOF
 * @param fr
 * @return 1 if EOF, else 0
 */
int FileReader_isEOF(FileReader* fr);

#endif //WINDSOLAR_FILE_READER_H

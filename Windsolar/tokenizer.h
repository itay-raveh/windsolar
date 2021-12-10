//
// Created by itay on 11/29/21.
//

#ifndef WINDSOLAR_TOKENIZER_H
#define WINDSOLAR_TOKENIZER_H

#include "file_reader.h"
#include "token.h"

typedef struct
{
    FileReader *fr;         // FileReader of file to tokenize
    int32_t in_block;       // is the tokenizer currently inside a command block
} Tokenizer;

/**
 * Dynamically allocate a Tokenizer and initialize the fields
 *
 * @param fr - FileReader to read from
 * @return ptr to Tokenizer
 */
Tokenizer *Tokenizer_init(FileReader *fr);

/**
 * Free a dynamically allocated Tokenizer
 *
 * @param t - Tokenizer
 */
void Tokenizer_free(Tokenizer *t);

/**
 * Get the next token from the file.
 * Return the token type, and set the pointers to the start and end of it in the buffer.
 *
 * @param t - Tokenizer
 * @param str - At return this will point to the start of the token
 * @param len - At return this will be the length of the token
 * @return Token type
 */
Token Tokenizer_next(Tokenizer *t, char **str, size_t *len);

#endif //WINDSOLAR_TOKENIZER_H

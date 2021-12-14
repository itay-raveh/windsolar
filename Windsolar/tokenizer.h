//
// Created by itay on 11/29/21.
//

#ifndef WINDSOLAR_TOKENIZER_H
#define WINDSOLAR_TOKENIZER_H

#include "file_reader.h"
#include "token.h"
#include "error.h"

typedef struct
{
    FileReader *fr;         // FileReader of file to tokenize
    int32_t in_block;       // Is the tokenizer currently inside a command block
    Token token;            // Current token type
    char *str;              // Start of current token
    size_t len;             // Length of the current token
    Error err;              // Error type for when one is found
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
 * Get the next token from the input.
 *
 * @param t - Tokenizer
 * @return 1 on success, 0 on error
 */
int32_t Tokenizer_next(Tokenizer *t);

#endif //WINDSOLAR_TOKENIZER_H

//
// Created by itay on 11/29/21.
//

#ifndef WINDSOLAR_TOKENIZER_H
#define WINDSOLAR_TOKENIZER_H

#include <stdbool.h>
#include "file_reader.h"
#include "token.h"
#include "error.h"

typedef struct
{
    FileReader *fr;         // FileReader of file to tokenize
    bool in_block;          // Is the tokenizer currently inside a command block
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
Tokenizer *Tokenizer_init(FileReader *restrict fr);

/**
 * Free a dynamically allocated Tokenizer
 *
 * @param t - Tokenizer
 */
void Tokenizer_free(Tokenizer *restrict t);

/**
 * Get the next token from the input.
 *
 * @param t - Tokenizer
 * @return true on success, false on error
 */
bool Tokenizer_next(Tokenizer *restrict t);

#endif //WINDSOLAR_TOKENIZER_H

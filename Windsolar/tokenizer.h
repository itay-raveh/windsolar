//
// Created by itay on 11/29/21.
//

#ifndef WINDSOLAR_TOKENIZER_H
#define WINDSOLAR_TOKENIZER_H

#include <stdbool.h>
#include "reader.h"
#include "token.h"
#include "error.h"

typedef struct
{
    Reader *reader;         // Reader of input to tokenize
    Token token;            // Current token type
    char *str;              // Start of current token
    uint32_t len;           // Length of the current token
    Error err;              // Error type for when one is found
    uint64_t lineno;        // Current line number
    uint64_t charno;        // Current char number relative to start of line
} Tokenizer;

/**
 * Dynamically allocate a Tokenizer and initialize the fields
 *
 * @param r - Reader to read from
 * @return ptr to Tokenizer
 */
Tokenizer *Tokenizer_fromReader(Reader *restrict r);

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

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
    int32_t lineno;         // line number
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
 * Get the next token from the file
 *
 * @param t - Tokenizer
 * @return Token, or NULL when reached EOF
 */
Token *Tokenizer_next(Tokenizer *t);

#endif //WINDSOLAR_TOKENIZER_H

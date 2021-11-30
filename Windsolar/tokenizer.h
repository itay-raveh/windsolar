//
// Created by itay on 11/29/21.
//

#ifndef WINDSOLAR_TOKENIZER_H
#define WINDSOLAR_TOKENIZER_H

#include "file_reader.h"
#include "token.h"

typedef struct
{
    FileReader *fr;     // FileReader of file to tokenize
    int in_block;       // is the tokenizer currently inside a command block
    int lineno;         // line number
} Tokenizer;

/**
 * Dynamically allocate a Tokenizer and initialize the fields
 *
 * @param fname - file to read from
 * @param fsize - size of file to read from
 * @return ptr to Tokenizer
 */
Tokenizer *Tokenizer_init(char *fname, size_t fsize);

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

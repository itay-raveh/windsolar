//
// Created by itay on 11/28/21.
//

#ifndef WINDSOLAR_TOKEN_H
#define WINDSOLAR_TOKEN_H

typedef enum
{
    LPAR, RPAR, SEMICOL, STRING, NUMBER, LABEL, CMD
} Token;

extern const char *const token_names[7];

#endif //WINDSOLAR_TOKEN_H

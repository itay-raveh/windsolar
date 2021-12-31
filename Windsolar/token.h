//
// Created by itay on 11/28/21.
//

#ifndef WINDSOLAR_TOKEN_H
#define WINDSOLAR_TOKEN_H

typedef enum
{
    T_LPAR, T_RPAR, T_SEMICOL, T_STRING, T_NUMBER, T_NAME, T_ENDMARKER
} Token;

extern const char *const token_names[];

#endif //WINDSOLAR_TOKEN_H

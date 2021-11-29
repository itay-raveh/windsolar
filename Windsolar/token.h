//
// Created by itay on 11/28/21.
//

#ifndef WINDSOLAR_TOKEN_H
#define WINDSOLAR_TOKEN_H

typedef enum
{
    PLUS        // +
} action_t;

typedef enum
{
    NAME,       // MAIN etc
    ACTION,     // action_t
    LPAR,       // (
    RPAR,       // )
    SEMICOL,    // ;
    NUM_LIT,    // 5 etc
    STR_LIT     // "Hello World" etc
} token_type_t;

typedef union
{
    char chr;       // LPAR, RPAR, SEMICOL
    char *str;      // NAME or STR_LIT
    int num;        // NUM_LIT
    action_t act;
} token_content_t;

typedef struct
{
    token_type_t type;
    token_content_t content;
} Token;

int Token_isContentChr(Token *t);

int Token_isContentStr(Token *t);

int Token_isContentNum(Token *t);

int Token_isContentAction(Token *t);

Token *Token_init(token_type_t type, token_content_t content);

void Token_free(Token *t);

#endif //WINDSOLAR_TOKEN_H

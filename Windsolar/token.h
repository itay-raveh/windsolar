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

/**
 * Dynamically allocate a new Token and initialize the fields.
 *
 * @param type
 * @param content
 * @return ptr to Token
 */
Token *Token_init(token_type_t type, token_content_t content);

/**
 * Free a dynamically allocated Token
 *
 * @param t - Token to free
 */
void Token_free(Token *t);

/**
 * Check the type of a Token to determine if it's content is a char
 *
 * @param t - Token
 * @return 1 if it is, otherwise 0
 */
int Token_isContentChr(Token *t);

/**
 * Check the type of a Token to determine if it's content is a string
 *
 * @param t - Token
 * @return 1 if it is, otherwise 0
 */
int Token_isContentStr(Token *t);

/**
 * Check the type of a Token to determine if it's content is a number
 *
 * @param t - Token
 * @return 1 if it is, otherwise 0
 */
int Token_isContentNum(Token *t);

/**
 * Check the type of a Token to determine if it's content is an action
 *
 * @param t - Token
 * @return 1 if it is, otherwise 0
 */
int Token_isContentAction(Token *t);

#endif //WINDSOLAR_TOKEN_H

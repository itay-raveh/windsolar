//
// Created by itay on 12/14/21.
//

#ifndef WINDSOLAR_ERROR_H
#define WINDSOLAR_ERROR_H

typedef enum
{
    E_OK,
    E_UNEXPECTED_CHAR,
    E_UNCLOSED_STRING,
    E_UNCLOSED_COMMENT
} Error;

extern const char *const error_msg[4];

#endif //WINDSOLAR_ERROR_H

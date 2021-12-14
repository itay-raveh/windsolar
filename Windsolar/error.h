//
// Created by itay on 12/14/21.
//

#ifndef WINDSOLAR_ERROR_H
#define WINDSOLAR_ERROR_H

typedef enum
{
    OK,
    UNEXPECTED_CHAR,
    UNCLOSED_STRING,
    UNCLOSED_COMMENT
} Error;

extern const char *const error_msg[4];

#endif //WINDSOLAR_ERROR_H

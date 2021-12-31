//
// Created by itay on 12/14/21.
//

#ifndef WINDSOLAR_ERROR_H
#define WINDSOLAR_ERROR_H

typedef enum
{
    E_OK,
    E_UNPARSABLE_NUMBER,
    E_UNCLOSED_STRING,
    E_UNCLOSED_COMMENT,
    E_MISSING_LABEL,
    E_MISSING_LPAR,
    E_MISSING_SEMICOL,
    E_UNCLOSED_BLOCK,
    E_SUB_BLOCKS_UNSUPPORTED
} Error;

extern const char *const error_msg[];

#endif //WINDSOLAR_ERROR_H

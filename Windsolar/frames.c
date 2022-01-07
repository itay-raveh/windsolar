//
// Created by itay on 1/6/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frames.h"
#include "utils.h"  // NEW(), realloc_s(), malloc_s()

char *string_repr(char *src, size_t len, size_t field_width)
{
    char *str = (char *) malloc_s(len + 3);
    size_t new_len = len + 2;

    if (new_len <= field_width) strncpy(&(str[1]), src, len);
    else
    {
        strncpy(&(str[1]), src, field_width);
        new_len = field_width;
        str[new_len - 2] = str[new_len - 3] = str[new_len - 4] = '.';
    }

    str[new_len - 1] = str[0] = '"';
    str[new_len] = '\0';

    return str;
}

DataFrame *DataFrame_new(const double *number, char *str)
{
    DataFrame *df = NEW(DataFrame);
    if (number != NULL)
    {
        df->isNumber = true;
        df->number = *number;
    } else
    {
        df->isNumber = false;
        df->str = str;
    }
    return df;
}

void DataFrame_free(DataFrame *df)
{
    if (!df->isNumber) free(df->str);
    free(df);
}

void DataFrame_print(DataFrame *df, uint16_t field_width)
{
    if (df->isNumber)
    {
        if ((int64_t) df->number == df->number)
            printf("%*ld", field_width * -1, (int64_t) df->number);
        else
            printf("%*f", field_width * -1, df->number);
    } else
    {
        char *str = string_repr(df->str, strlen(df->str), field_width);
        printf("%*.*s", field_width * -1, field_width, str);
        free(str);
    }
}

ProgramFrame *ProgramFrame_new(Token type, char *str)
{
    ProgramFrame *pf = NEW(ProgramFrame);
    pf->type = type;
    pf->str = str;
    return pf;
}

void ProgramFrame_free(ProgramFrame *pf)
{
    free(pf->str);
    free(pf);
}

void ProgramFrame_print(ProgramFrame *pf, uint16_t field_width)
{
    if (pf->type == T_STRING)
    {
        char *str = string_repr(pf->str, strlen(pf->str), field_width);
        printf("%*.*s", field_width * -1, field_width, str);
        free(str);
    }
    else
        printf("%*.*s", field_width * -1, field_width, pf->str);
}

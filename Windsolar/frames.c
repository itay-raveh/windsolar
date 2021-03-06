//
// Created by itay on 1/6/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "frames.h"
#include "utils.h"  // NEW(), realloc_s(), malloc_s()

char *string_repr(char *src, const size_t len, const size_t field_width)
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

DataFrame *DataFrame_new(const double *restrict number, char *const restrict str)
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

void DataFrame_free(DataFrame *restrict df)
{
    if (!df->isNumber) free(df->str);
    free(df);
}

void DataFrame_print(DataFrame *const restrict df, const uint16_t field_width)
{
    if (df->isNumber)
    {
        if (floor(df->number) == ceil(df->number))
            fprintf(stderr, "%*.0f", field_width * -1, df->number);
        else
            fprintf(stderr, "%*f", field_width * -1, df->number);
    } else
    {
        char *str = string_repr(df->str, strlen(df->str), field_width);
        fprintf(stderr, "%*.*s", field_width * -1, field_width, str);
        free(str);
    }
}

ProgramFrame *ProgramFrame_new(const Token type, char *const restrict str)
{
    ProgramFrame *pf = NEW(ProgramFrame);
    pf->type = type;
    pf->str = str;
    return pf;
}

void ProgramFrame_free(ProgramFrame *const restrict pf)
{
    free(pf->str);
    free(pf);
}

void ProgramFrame_print(ProgramFrame *const restrict pf, const uint16_t field_width)
{
    if (pf->type == T_STRING)
    {
        char *str = string_repr(pf->str, strlen(pf->str), field_width);
        fprintf(stderr, "%*.*s", field_width * -1, field_width, str);
        free(str);
    } else
        fprintf(stderr, "%*.*s", field_width * -1, field_width, pf->str);
}

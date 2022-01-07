//
// Created by itay on 1/6/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "frames.h"
#include "utils.h"  // NEW(), realloc_s(), malloc_s()

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

void DataFrame_print(DataFrame *df, int16_t field_width)
{
    if (df->isNumber)
    {
        if ((int64_t) df->number == df->number)
            printf("%*ld", field_width * -1, (int64_t) df->number);
        else
            printf("%*f", field_width * -1, df->number);
    } else
        printf("\"%*s\"", (field_width - 2) * -1, df->str);
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

void ProgramFrame_print(ProgramFrame *pf, int16_t field_width)
{
    if (pf->type == T_STRING)
        printf("\"%*s\"", (field_width - 2) * -1, pf->str);
    else
        printf("%*s", field_width * -1, pf->str);
}

//
// Created by itay on 1/6/22.
//

#ifndef WINDSOLAR_FRAMES_H
#define WINDSOLAR_FRAMES_H

#include <stdbool.h>
#include <inttypes.h>
#include "token.h"

typedef struct data_frame_t
{
    bool isNumber;
    double number;
    char *str;
} DataFrame;

DataFrame *DataFrame_new(const double *number, char *str);
void DataFrame_free(DataFrame *df);
void DataFrame_print(DataFrame *df, uint16_t field_width);

typedef struct program_frame_t
{
    Token type;
    char *str;
} ProgramFrame;

ProgramFrame *ProgramFrame_new(Token type, char *str);
void ProgramFrame_free(ProgramFrame *pf);
void ProgramFrame_print(ProgramFrame *pf, uint16_t field_width);

#endif //WINDSOLAR_FRAMES_H

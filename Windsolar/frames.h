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

DataFrame *DataFrame_new(const double *restrict number, char *restrict str);

void DataFrame_free(DataFrame *restrict df);

void DataFrame_print(DataFrame *restrict df, uint16_t field_width);

typedef struct program_frame_t
{
    Token type;
    char *str;
} ProgramFrame;

ProgramFrame *ProgramFrame_new(Token type, char *restrict str);

void ProgramFrame_free(ProgramFrame *restrict pf);

void ProgramFrame_print(ProgramFrame *restrict pf, uint16_t field_width);

#endif //WINDSOLAR_FRAMES_H

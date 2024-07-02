#ifndef EL_VARIABLE_H
#define EL_VARIABLE_H

#include <el_structure.h>

typedef struct
{
    char name[NAME_LENGTH];
    char second_name[NAME_LENGTH];
    char tel[NAME_LENGTH];
} abonent;

extern int abonent_count;
extern abonent abonents[MAX_ABONENTS];

#endif // EL_VARIABLE_H
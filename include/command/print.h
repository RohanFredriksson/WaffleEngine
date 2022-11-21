#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"
#include "command.h"

#ifndef PRINT_H
#define PRINT_H

struct Print {
    Command* command;
    char* message;
};
typedef struct Print Print;

Command* Print_Init(char* message);

void Print_Execute(Command* a, Component* c);

void Print_Free(Command* a);

#endif
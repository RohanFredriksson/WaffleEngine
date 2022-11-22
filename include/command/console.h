#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "command.h"

#ifndef CONSOLE_H
#define CONSOLE_H

struct Console {
    Command* command;
    char* message;
};
typedef struct Console Console;

Command* Console_Init(char* message);

void Console_Execute(Command* a, Component* c);

cJSON* Console_Serialise(Command* a);

void Console_Free(Command* a);

#endif
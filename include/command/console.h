#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void Console_Free(Command* a);

#endif
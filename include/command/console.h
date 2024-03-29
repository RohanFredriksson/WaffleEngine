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

bool Console_Load(Command* a, cJSON* json);

#endif
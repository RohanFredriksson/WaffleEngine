#include <string.h>
#include "external.h"
#include "assetpool.h"
#include "entity.h"
#include "wio.h"

#ifndef COMMAND_H
#define COMMAND_H

struct Command {
    void (*execute) (struct Command* a, Component* c);
    cJSON* (*serialise) (struct Command* a);
    void (*free) (struct Command* a);
    char* type;
    void* data;
};
typedef struct Command Command;

Command* Command_Init(char* type);

void Command_Execute(Command* a, Component* c);

cJSON* Command_Serialise(Command* a);

Command* Command_Load(cJSON* json);

void Command_Free(Command* a);

#endif
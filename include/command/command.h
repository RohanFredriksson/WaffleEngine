#include <string.h>
#include "gameobject.h"

#ifndef COMMAND_H
#define COMMAND_H

struct Command {
    void (*execute) (struct Command* a, Component* c);
    void (*free)(struct Command* a);
    char* type;
    void* data;
};
typedef struct Command Command;

Command* Command_Init(char* type, 
                      void (*execute) (struct Command* a, Component* c), 
                      void (*free)(struct Command* a));

void Command_Execute(Command* a, Component* c);

void Command_Free(Command* a);

#endif
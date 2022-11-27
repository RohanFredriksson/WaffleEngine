#include "entity.h"
#include "command.h"

#ifndef DELETETHISENTITY_H
#define DELETETHISENTITY_H

Command* DeleteThisEntity_Init();
bool DeleteThisEntity_Load(Command* command, cJSON* json);

#endif
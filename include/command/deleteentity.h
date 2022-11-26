#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "entity.h"
#include "command.h"

#ifndef DELETEENTITY_H
#define DELETEENTITY_H

struct DeleteEntity {
    Command* command;
    int id;
};
typedef struct DeleteEntity DeleteEntity;

Command* DeleteEntity_Init(Entity* e);

bool DeleteEntity_Load(Command* a, cJSON* json);

#endif
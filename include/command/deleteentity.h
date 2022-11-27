#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "entity.h"
#include "command.h"

#ifndef DELETEENTITY_H
#define DELETEENTITY_H

#define DELETE_TYPE_ID 0
#define DELETE_TYPE_THIS 1

struct DeleteEntity {
    Command* command;
    bool deleteType;
    int id;
};
typedef struct DeleteEntity DeleteEntity;

Command* DeleteEntity_Init(Entity* e);

Command* DeleteThisEntity_Init();

bool DeleteEntity_Load(Command* a, cJSON* json);

#endif
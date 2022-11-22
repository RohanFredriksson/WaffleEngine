#include <stdbool.h>
#include "external.h"
#include "entity.h"
#include "event.h"

#ifndef TRIGGER_H
#define TRIGGER_H

#define TRIGGER_WITH_ANY 0
#define TRIGGER_WITH_ENTITY 1
#define TRIGGER_WITH_COMPONENT_TYPE 2

struct Trigger {
    Event* event;
    int withType;
    int entityId;
    char* componentType;
    bool flag;
};
typedef struct Trigger Trigger;

Component* Trigger_Init();

Component* TriggerOnEntity_Init(Entity* entity);

Component* TriggerOnComponentType_Init(char* type);

bool Trigger_Check(Event* e, float dt);

void Trigger_OnCollision(Event* e, Entity* with, vec2 contact, vec2 normal);

#endif
#include <stdbool.h>
#include "external.h"
#include "gameobject.h"
#include "event.h"

#ifndef TRIGGER_H
#define TRIGGER_H

struct Trigger {
    Event* event;
    bool flag;
};
typedef struct Trigger Trigger;

Component* Trigger_Init();

bool Trigger_Check(Event* e, float dt);

void Trigger_OnCollision(Event* e, GameObject* with, vec2 contact, vec2 normal);

#endif
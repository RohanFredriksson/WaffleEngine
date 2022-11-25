#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "entity.h"
#include "command.h"
#include "list.h"
#include "wio.h"

#ifndef EVENT_H
#define EVENT_H

#define EVENT_SINGLE_USE 0
#define EVENT_MULTIPLE_USE 1

struct Event {
    Component* component;
    List commands;
    bool multi;
    float cooldown;
    float cooldownTimeLeft;
    char* type;
    bool (*check)(struct Event* e, float dt);
    void (*collision) (struct Event* e, Entity* with, vec2 contact, vec2 normal);
    cJSON* (*serialise) (struct Event* e);
    void (*free)(struct Event* e);
    void* data;
};
typedef struct Event Event;

Component* Event_Init(char* type);

bool Event_Load(Component* c, cJSON* json);

void Event_AddCommand(Component* c, Command* a);

void Event_SetCommand(Component* c, Command* a);

void Event_SetMulti(Component* c, bool multi);

void Event_SetCooldown(Component* c, float cooldown);

#endif
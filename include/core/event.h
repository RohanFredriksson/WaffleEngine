#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "gameobject.h"
#include "command.h"
#include "list.h"

#ifndef EVENT_H
#define EVENT_H

struct Event {
    Component* component;
    List commands;
    bool active;
    bool multi;
    float cooldown;
    float cooldownTimeLeft;
    char* type;
    bool (*check)(struct Event* e, float dt);
    void (*free)(struct Event* e);
    void* data;
};
typedef struct Event Event;

Component* Event_Init(char* type, 
                      bool (*check)(Event* e, float dt), 
                      void (*free)(Event* e));

void Event_Update(Component* c, float dt);

void Event_Free(Component* c);

void Event_AddCommand(Component* c, Command* a);

void Event_SetCommand(Component* c, Command* a);

void Event_SetActive(Component* c, bool active);

void Event_SetMulti(Component* c, bool multi);

void Event_SetCooldown(Component* c, float cooldown);

#endif
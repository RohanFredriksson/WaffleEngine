#include "event.h"

Component* Event_Init(char* type, 
                      bool (*check)(Event* e, float dt), 
                      void (*collision) (Event* e, Entity* with, vec2 contact, vec2 normal),
                      cJSON* (*serialise) (Event* e), 
                      void (*free)(Event* e)) {

    Component* c = Component_Init("Event");
    Event* e = malloc(sizeof(Event));
    e->type = type;
    e->check = check;
    e->collision = collision;
    e->serialise = serialise;
    e->free = free;
    e->component = c;
    List_Init(&e->commands, sizeof(Command*));
    e->multi = 1;
    e->cooldown = 0.0f;
    e->cooldownTimeLeft = 0.0f;

    c->update = &Event_Update;
    c->collision = &Event_OnCollision;
    c->serialise = &Event_Serialise;
    c->free = &Event_Free;
    c->data = e;

    return c;
}

void Event_Update(Component* c, float dt) {

    Event* e = (Event*) c->data;

    // If the command can be run multiple times.
    if (e->multi) {

        if (e->cooldownTimeLeft > 0) {
            e->cooldownTimeLeft -= dt;
            return;
        }

        if (e->check != NULL) {
            if (e->check(e, dt)) {

                // Execute all commands.
                Command* command;
                int n = List_Length(&e->commands);
                for (int i = 0; i < n; i++) {
                    List_Get(&e->commands, i, &command);
                    Command_Execute(command, c);
                }

                // Set the cooldown.
                e->cooldownTimeLeft = e->cooldown;

            }
        }

    // If the command is only meant to be run once.
    } else {

        if (e->check != NULL) {
            if (e->check(e, dt)) {

                // Execute all commands.
                Command* command;
                int n = List_Length(&e->commands);
                for (int i = 0; i < n; i++) {
                    List_Get(&e->commands, i, &command);
                    Command_Execute(command, c);
                }

                // Remove the object.
                Entity_RemoveComponent(c->entity, c);

            }
        }
    }
}

void Event_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal) {
    Event* e = (Event*) c->data;
    if (e->collision != NULL) {e->collision(e, with, contact, normal);}
}

cJSON* Event_Serialise(Component* c) {

    Event* e = (Event*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "type", e->type);
    cJSON* child;
    if (e->serialise != NULL) {child = e->serialise(e);}
    else {child = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "child", child);
    cJSON* commands = cJSON_CreateArray();
    Command* com;
    int n = List_Length(&e->commands);
    for (int i = 0; i < n; i++) {
        List_Get(&e->commands, i, &com);
        cJSON* command = Command_Serialise(com);
        cJSON_AddItemToArray(commands, command);
    }
    cJSON_AddItemToObject(json, "commands", commands);
    WIO_AddBool(json, "multi", e->multi);
    WIO_AddFloat(json, "cooldown", e->cooldown);
    WIO_AddFloat(json, "cooldownTimeLeft", e->cooldownTimeLeft);
    return json;

}

void Event_Free(Component* c) {

    Event* e = (Event*) c->data;

    // Free all commands.
    Command* command;
    int n = List_Length(&e->commands);
    for (int i = 0; i < n; i++) {
        List_Get(&e->commands, i, &command);
        Command_Free(command);
        free(command);
    }
    List_Free(&e->commands);

    // Free all event memory
    if (e->free != NULL) {e->free(e);}
    if (e->data != NULL) {free(e->data);}

}

void Event_AddCommand(Component* c, Command* a) {
    Event* e = (Event*) c->data;
    List_Push(&e->commands, &a);
}

void Event_SetCommand(Component* c, Command* a) {
    Event* e = (Event*) c->data;
    List_Clear(&e->commands);
    List_Push(&e->commands, &a);
}

void Event_SetMulti(Component* c, bool multi) {
    Event* e = (Event*) c->data;
    e->multi = multi;
}

void Event_SetCooldown(Component* c, float cooldown) {
    Event* e = (Event*) c->data;
    e->cooldown = cooldown;
}
#include "event.h"
#include "events.h"

Component* Event_Init(char* type) {

    Component* c = Component_Init("Event");

    Event* e = malloc(sizeof(Event));
    e->check = NULL;
    e->collision = NULL;
    e->serialise = NULL;
    e->free = NULL;

    e->type = type;
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

bool Event_Load(Component* c, cJSON* json) {

    bool multi;
    float cooldown;
    float cooldownTimeLeft;
    char* type;

    if (!WIO_ParseBool(json, "multi", &multi)) {return 0;}
    if (!WIO_ParseFloat(json, "cooldown", &cooldown)) {return 0;}
    if (!WIO_ParseFloat(json, "cooldownTimeLeft", &cooldownTimeLeft)) {return 0;}
    if (!WIO_ParseString(json, "type", &type)) {return 0;}

    Event* e = malloc(sizeof(Event));
    e->check = NULL;
    e->collision = NULL;
    e->serialise = NULL;
    e->free = NULL;

    cJSON* child = cJSON_GetObjectItemCaseSensitive(json, "child");
    if (child == NULL) {free(e); return 0;}
    if (strcmp(type, "MouseButtonDown") == 0) {if (!MouseButton_Load(e, child)) {free(e); return 0;}}
    else if (strcmp(type, "Trigger") == 0) {if (!Trigger_Load(e, child)) {free(e); return 0;}}
    else {free(e); return 0;}

    e->type = type;
    e->component = c;
    List_Init(&e->commands, sizeof(Command*));
    e->multi = multi;
    e->cooldown = cooldown;
    e->cooldownTimeLeft = cooldownTimeLeft;

    cJSON* commands = cJSON_GetObjectItemCaseSensitive(json, "commands");
    if (commands != NULL && cJSON_IsArray(commands)) {
        cJSON* command = NULL;
        cJSON_ArrayForEach(command, commands) {
            Command* com = Command_Load(command);
            if (com != NULL) {List_Push(&e->commands, &com);}
        }
    }

    c->update = &Event_Update;
    c->collision = &Event_OnCollision;
    c->serialise = &Event_Serialise;
    c->free = &Event_Free;
    c->data = e;

    return 1;
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
#include "event.h"

Component* Event_Init(char* type, 
                      bool (*check)(Event* e, float dt), 
                      void (*collision) (struct Event* e, Entity* with, vec2 contact, vec2 normal), 
                      void (*free)(Event* e)) {

    Component* c = Component_Init("Event", &Event_Update, &Event_OnCollision, &Event_Free);
    Event* e = malloc(sizeof(Event));

    e->type = type;
    e->check = check;
    e->collision = collision;
    e->free = free;

    e->component = c;

    List_Init(&e->commands, sizeof(Command*));
    e->active = 1;
    e->multi = 1;
    e->cooldown = 0.0f;
    e->cooldownTimeLeft = 0.0f;

    c->data = e;
    return c;

}

void Event_Update(Component* c, float dt) {

    Event* e = (Event*) c->data;
    if (!e->active) {return;}

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
    free(e->type);

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

void Event_SetActive(Component* c, bool active) {
    Event* e = (Event*) c->data;
    e->active = active;
}

void Event_SetMulti(Component* c, bool multi) {
    Event* e = (Event*) c->data;
    e->multi = multi;
}

void Event_SetCooldown(Component* c, float cooldown) {
    Event* e = (Event*) c->data;
    e->cooldown = cooldown;
}
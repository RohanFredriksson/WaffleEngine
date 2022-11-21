#include "trigger.h"

Component* Trigger_Init() {

    Component* component = Event_Init("Trigger", &Trigger_Check, &Trigger_OnCollision, NULL);
    Event* event = (Event*) component->data;
    Trigger* trigger = malloc(sizeof(Trigger));

    trigger->event = event;
    trigger->flag = 0;

    event->data = trigger;
    return component;

}

bool Trigger_Check(Event* e, float dt) {
    
    Trigger* t = (Trigger*) e->data;
    if (t->flag) {
        t->flag = 0;
        return 1;
    }

    return 0;
}

void Trigger_OnCollision(Event* e, Entity* with, vec2 contact, vec2 normal) {
    Trigger* t = (Trigger*) e->data;
    t->flag = 1;
}

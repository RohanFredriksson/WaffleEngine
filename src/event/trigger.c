#include "trigger.h"

static bool Trigger_Check(Event* e, float dt) {
    
    Trigger* t = (Trigger*) e->data;
    if (t->flag) {
        t->flag = 0;
        return 1;
    }

    return 0;
}

static void Trigger_OnCollision(Event* e, Entity* with, vec2 contact, vec2 normal) {
    
    Trigger* t = (Trigger*) e->data;

    // Trigger on entity
    if (t->withType == TRIGGER_WITH_ENTITY) {
        if (t->entityId == with->id) {t->flag = 1;}
    }

    // Trigger on component type
    else if (t->withType == TRIGGER_WITH_COMPONENT_TYPE) {
        if (Entity_GetComponent(with, t->componentType) != NULL) {t->flag = 1;}
    }

    // Trigger on anything
    else {
        t->flag = 1;
    }

}

static cJSON* Trigger_Serialise(Event* e) {

    Trigger* t = (Trigger*) e->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "withType", t->withType);
    WIO_AddInt(json, "entityId", t->entityId);
    cJSON* componentType;
    if (t->componentType != NULL) {componentType = cJSON_CreateString(t->componentType);}
    else {componentType = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "componentType", componentType);
    WIO_AddBool(json, "flag", t->flag);
    return json;

}

static Component* _Trigger_Init(int withType, int entityId, char* componentType) {

    Component* component = Event_Init("Trigger");
    Event* event = (Event*) component->data;
    Trigger* trigger = malloc(sizeof(Trigger));
    trigger->event = event;
    trigger->withType = withType;
    trigger->entityId = entityId;
    trigger->componentType = componentType;
    trigger->flag = 0;

    event->check = &Trigger_Check;
    event->collision = &Trigger_OnCollision;
    event->serialise = &Trigger_Serialise;
    event->data = trigger;
    return component;

}

Component* Trigger_Init() {
    return _Trigger_Init(TRIGGER_WITH_ANY, -1, NULL);
}

Component* TriggerOnEntity_Init(Entity* entity) {
    return _Trigger_Init(TRIGGER_WITH_ENTITY, entity->id, NULL);
}

Component* TriggerOnComponentType_Init(char* type) {
    return _Trigger_Init(TRIGGER_WITH_COMPONENT_TYPE, -1, type);
}

bool Trigger_Load(Event* e, cJSON* json) {

    int withType;
    int entityId;
    bool flag;
    char* componentType;

    if (!WIO_ParseInt(json, "withType", &withType)) {return 0;}
    if (!WIO_ParseInt(json, "entityId", &entityId)) {return 0;}
    if (!WIO_ParseBool(json, "flag", &flag)) {return 0;}
    if (!WIO_ParseString(json, "componentType", &componentType)) {return 0;}

    Trigger* trigger = malloc(sizeof(Trigger));
    trigger->event = e;
    trigger->withType = withType;
    trigger->entityId = entityId;
    trigger->componentType = componentType;
    trigger->flag = flag;

    e->check = &Trigger_Check;
    e->collision = &Trigger_OnCollision;
    e->serialise = &Trigger_Serialise;
    e->data = trigger;
    return 1;

}
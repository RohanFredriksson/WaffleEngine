#include "rigidbody.h"
#include "colliders.h"

static void Collider_Update(Component* c, float dt) {
    Collider* co = (Collider*) c->data;
    if (co->update != NULL) {co->update(co, dt);}
}

static void Collider_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal) {
    Collider* co = (Collider*) c->data;
    if (co->collision != NULL) {co->collision(co, with, contact, normal);}
}

static cJSON* Collider_Serialise(Component* c) {

    Collider* co = (Collider*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "type", co->type);
    cJSON* child;
    if (co->serialise != NULL) {child = co->serialise(co);}
    else {child = cJSON_CreateNull();} 
    cJSON_AddItemToObject(json, "child", child);
    WIO_AddInt(json, "rigidbody", co->rigidbody);
    return json;

}

static void Collider_Free(Component* c) {
    Collider* co = (Collider*) c->data;
    if (co->free != NULL) {co->free(co);}
    if (co->data != NULL) {free(co->data);}
}

static Collider* _Collider_Init(Component* c, char* type, int rigidbody) {

    Collider* co = malloc(sizeof(Collider));
    co->update = NULL;
    co->collision = NULL;
    co->serialise = NULL;
    co->free = NULL;

    co->component = c;
    co->type = type;
    co->rigidbody = rigidbody;
    
    c->update = &Collider_Update;
    c->collision = &Collider_OnCollision;
    c->serialise = &Collider_Serialise;
    c->free = &Collider_Free;
    c->data = co;

    return co;
}

Component* Collider_Init(char* type) {
    Component* c = Component_Init("Collider");
    _Collider_Init(c, type, -1);
    return c;
}

bool Collider_Load(Component* c, cJSON* json) {

    int rigidbody;
    char* type;

    if (!WIO_ParseInt(json, "rigidbody", &rigidbody)) {return 0;}
    if (!WIO_ParseString(json, "type", &type)) {return 0;}

    // Initialise the collider class.
    Collider* co = _Collider_Init(c, type, rigidbody);

    // Attempt to parse the child class. If cannot, free the event entirely.
    cJSON* child = cJSON_GetObjectItemCaseSensitive(json, "child");
    if (child == NULL) {free(co); return 0;}
    if (strcmp(type, "Box") == 0) {if (!Box_Load(co, child)) {free(co); return 0;}}
    else if (strcmp(type, "Circle") == 0) {if (!Circle_Load(co, child)) {free(co); return 0;}}
    else {free(co); return 0;}

    return 1;

}

Component* Collider_GetRigidbody(Collider* co) {
    return Entity_GetComponentByID(co->component->entity, co->rigidbody);
}

void Collider_SetRigidbody(Component* c, Component* rigidbody) {

    if (strcmp(c->type, "Collider") != 0) {
        printf("ERROR::COLLIDER_SETRIGIDBODY::COMPONENT_NOT_OF_COLLIDER_TYPE\n");
        return;
    }

    if (strcmp(rigidbody->type, "Rigidbody") != 0) {
        printf("ERROR::COLLIDER_SETRIGIDBODY::COMPONENT_NOT_OF_RIGIDBODY_TYPE");
        return;
    }

    ((Collider*) c->data)->rigidbody = rigidbody->id;

}
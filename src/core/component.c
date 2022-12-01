#include "entity.h"
#include "components.h"

static int next = 0;

Component* _Component_Init(int id, bool dead, char* type, vec2 positionOffset, vec2 sizeScale, float rotationOffset) {

    Component* c = (Component*) malloc(sizeof(Component));

    c->update = NULL;
    c->collision = NULL;
    c->serialise = NULL;
    c->free = NULL;

    c->id = id;
    c->dead = dead;
    c->ignore = 0;
    c->type = type;
    glm_vec2_copy(positionOffset, c->positionOffset);
    glm_vec2_copy(sizeScale, c->sizeScale);
    c->rotationOffset = rotationOffset;

    next = WMath_MaxFloat(next+1, id+1);
    return c;
}

Component* Component_Init(char* type) {
    Component* c = _Component_Init(next, 0, type, (vec2) {0,0}, (vec2) {1,1}, 0);
    return c;
}

void Component_Update(Component* c, float dt) {
    if (c->update != NULL) {c->update(c, dt);}
}

void Component_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal) {
    if (c->collision != NULL) {c->collision(c, with, contact, normal);}
}

void Component_Free(Component* c) {
    if (c->free != NULL) {c->free(c);}
    if (c->data != NULL) {free(c->data);}
}

void Component_Kill(Component* c) {
    c->dead = 1; // Flags for the entity to remove this component at the end of its update call.
}

void Component_Ignore(Component* c) {
    c->ignore = 1;
}

void Component_Unignore(Component* c) {
    c->ignore = 0;
}

void Component_GetPosition(Component* c, vec2 dest) {
    glm_vec2_add(c->entity->position, c->positionOffset, dest);
}

void Component_GetSize(Component* c, vec2 dest) {
    glm_vec2_mul(c->entity->size, c->sizeScale, dest);
}

float Component_GetRotation(Component* c) {
    return c->entity->rotation + c->rotationOffset;
}

void Component_GetPositionOffset(Component* c, vec2 dest) {
    glm_vec2_copy(c->positionOffset, dest);
}

void Component_GetSizeScale(Component* c, vec2 dest) {
    glm_vec2_copy(c->sizeScale, dest);
}

float Component_GetRotationOffset(Component* c) {
    return c->rotationOffset;
}

void Component_SetPositionOffset(Component* c, vec2 offset) {
    glm_vec2_copy(offset, c->positionOffset);
}

void Component_SetSizeScale(Component* c, vec2 scale) {
    glm_vec2_copy(scale, c->sizeScale);
}

void Component_SetRotationOffset(Component* c, float offset) {
    c->rotationOffset = offset;
}

cJSON* Component_Serialise(Component* c) {

    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "id", c->id);
    WIO_AddString(json, "type", c->type);
    WIO_AddBool(json, "dead", c->dead);
    cJSON* child;
    if (c->serialise != NULL) {child = c->serialise(c);}
    else {child = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "child", child);
    WIO_AddVec2(json, "positionOffset", c->positionOffset);
    WIO_AddVec2(json, "sizeScale", c->sizeScale);
    WIO_AddFloat(json, "rotationOffset", c->rotationOffset);
    return json;

}

Component* Component_Load(cJSON* json) {

    int id;
    char* type;
    bool dead;
    vec2 positionOffset;
    vec2 sizeScale;
    float rotationOffset;

    if (!WIO_ParseInt(json, "id", &id)) {printf("A\n"); return NULL;}
    if (!WIO_ParseBool(json, "dead", &dead)) {dead = 0;}
    if (!WIO_ParseVec2(json, "positionOffset", positionOffset)) {glm_vec2_zero(positionOffset);}
    if (!WIO_ParseVec2(json, "sizeScale", sizeScale)) {glm_vec2_one(sizeScale);}
    if (!WIO_ParseFloat(json, "rotationOffset", &rotationOffset)) {rotationOffset = 0.0f;}
    if (!WIO_ParseString(json, "type", &type)) {printf("F\n");return NULL;}
    if (type == NULL) {return NULL;}

    // Initialise the component class.
    Component* c = _Component_Init(id, dead, type, positionOffset, sizeScale, rotationOffset);

    // Attempt to parse the child class. If cannot, free the component entirely.
    cJSON* child = cJSON_GetObjectItemCaseSensitive(json, "child");
    if (child == NULL) {free(c); return NULL;}
    if (strcmp(type, "SpriteRenderer") == 0) {if (!SpriteRenderer_Load(c, child)) {free(c); return NULL;}}
    else if (strcmp(type, "Collider") == 0) {if (!Collider_Load(c, child)) {free(c); return NULL;}}
    else if (strcmp(type, "Rigidbody") == 0) {if (!Rigidbody_Load(c, child)) {free(c); return NULL;}}
    else if (strcmp(type, "Event") == 0) {if (!Event_Load(c, child)) {free(c); return NULL;}}
    else if (strcmp(type, "TextRenderer") == 0) {if (!TextRenderer_Load(c, child)) {free(c); return NULL;}}
    else if (strcmp(type, "CameraController") == 0) {if (!CameraController_Load(c, child)) {free(c); return NULL;}}
    else {free(c); return NULL;}

    return c;
}
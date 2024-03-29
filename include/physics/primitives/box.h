#include "external.h"
#include "rigidbody.h"
#include "wio.h"

#ifndef BOX_H
#define BOX_H

struct Box {
    Collider* collider;
    vec2 size;
    vec2 halfSize;
};
typedef struct Box Box;

Component* Box_Init(vec2 size);
bool Box_Load(Collider* co, cJSON* json);
Component* Box_GetRigidbody(Box* box);
void Box_GetMin(Box* box, vec2 dest);
void Box_GetMax(Box* box, vec2 dest);
void Box_GetVertices(Box* box, vec2* buffer);
void Box_SetSize(Box* box, vec2 size);

#endif
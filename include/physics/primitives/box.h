#include "external.h"
#include "rigidbody.h"

#ifndef BOX_H
#define BOX_H

struct Box {
    Collider* collider;
    int rigidbody;
    vec2 size;
    vec2 halfSize;
};
typedef struct Box Box;

Component* Box_Init(vec2 size, Component* rigidbody);
cJSON* Box_Serialise(Collider* co);

Component* Box_GetRigidbody(Box* box);
void Box_GetMin(Box* box, vec2 dest);
void Box_GetMax(Box* box, vec2 dest);
void Box_GetVertices(Box* box, vec2* buffer);

void Box_SetSize(Box* box, vec2 size);
void Box_SetRigidbody(Box* box, Rigidbody* rb);

#endif
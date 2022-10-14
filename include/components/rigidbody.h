#include "external.h"
#include "gameobject.h"
#include "transform.h"
#include "bodytype.h"

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

struct RigidBody {

    Component* component;
    Transform* transform;
    BodyType bodyType;
    vec2 velocity;

};
typedef struct RigidBody RigidBody;

Component* RigidBody_Init(Transform* transform, BodyType bodyType);

void RigidBody_Update(Component* c, float dt);

void RigidBody_Free(Component* c);

#endif
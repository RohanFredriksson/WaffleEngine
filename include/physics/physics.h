#include "external.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "bodytype.h"

#ifndef PHYSICS_H
#define PHYSICS_H

struct Physics {
    vec2 gravity;
    RigidBody** bodies;
    int numBodies;
    int sizeBodies;
};
typedef struct Physics Physics;

void Physics_Init(Physics* p, vec2 gravity);

void Physics_Update(Physics* p, float dt);

void Physics_Free(Physics* p);

void Physics_AddGameObject(Physics* p, GameObject* go);

void Physics_AddRigidBody(Physics* p, RigidBody* rb);

#endif
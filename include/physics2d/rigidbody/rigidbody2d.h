#include <stdbool.h>
#include "external.h"
#include "transform.h"
#include "gameobject.h"

#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H

struct Rigidbody2D {
    
    Component* component;
    Transform* transform;
    float mass;
    vec2 forceAccum;
    vec2 linearVelocity;
    float angularVelocity;
    float linearDamping;
    float angularDamping;
    bool fixedRotation;
    
};
typedef struct Rigidbody2D Rigidbody2D;

Component* Rigidbody2D_Init(Transform* transform);

void Rigidbody2D_Update(Component* c, float dt);

void Rigidbody2D_Free(Component* c);

void Rigidbody2D_ClearAccumulators(Rigidbody2D* rb);

void Rigidbody2D_PhysicsUpdate(Rigidbody2D* rb, float dt);

void Rigidbody2D_AddForce(Rigidbody2D* rb, vec2 force);

#endif
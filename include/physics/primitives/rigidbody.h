#include <stdbool.h>
#include "external.h"
#include "transform.h"
#include "gameobject.h"
#include "collider.h"

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

struct Rigidbody {
    
    Component* component;
    Collider* collider;
    Transform* transform;
    float mass;
    vec2 forceAccum;
    vec2 velocity;
    bool sensor;
    float cor;
    
};
typedef struct Rigidbody Rigidbody;

Component* Rigidbody_Init(Transform* transform);

void Rigidbody_SetCollider(Component* c, Component* collider);

void Rigidbody_SetMass(Component* c, float mass);

void Rigidbody_SetVelocity(Component* c, vec2 velocity);

void Rigidbody_SetSensor(Component* c, bool sensor);

void Rigidbody_SetCor(Component* c, float cor);

void Rigidbody_ClearAccumulators(Rigidbody* rb);

void Rigidbody_PhysicsUpdate(Rigidbody* rb, float dt);

void Rigidbody_AddForce(Rigidbody* rb, vec2 force);

bool Rigidbody_HasInfiniteMass(Rigidbody* rb);

#endif
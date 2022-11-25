#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "external.h"
#include "entity.h"
#include "wio.h"

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

struct Rigidbody {
    
    Component* component;
    int collider;
    vec2 forceAccum;
    vec2 velocity;
    float cor;
    float mass;
    bool sensor;
    
};
typedef struct Rigidbody Rigidbody;

Component* Rigidbody_Init();

bool Rigidbody_Load(Component* c, cJSON* json);

void Rigidbody_SetCollider(Component* c, Component* collider);

void Rigidbody_SetMass(Component* c, float mass);

void Rigidbody_SetVelocity(Component* c, vec2 velocity);

void Rigidbody_SetSensor(Component* c, bool sensor);

void Rigidbody_SetCor(Component* c, float cor);

Component* Rigidbody_GetCollider(Rigidbody* rb);

void Rigidbody_ClearAccumulators(Rigidbody* rb);

void Rigidbody_PhysicsUpdate(Rigidbody* rb, float dt);

void Rigidbody_AddForce(Rigidbody* rb, vec2 force);

bool Rigidbody_HasInfiniteMass(Rigidbody* rb);

struct Collider {
    Component* component;
    int rigidbody;
    void (*update)(struct Collider* c, float dt);
    void (*collision)(struct Collider* c, Entity* with, vec2 contact, vec2 normal);
    cJSON* (*serialise)(struct Collider* c);
    void (*free)(struct Collider* c);
    char* type;
    void* data;
};
typedef struct Collider Collider;

Component* Collider_Init(char* type);

bool Collider_Load(Component* c, cJSON* json);

Component* Collider_GetRigidbody(Collider* co);

void Collider_SetRigidbody(Component* c, Component* rigidbody);

//float Collider_GetInertiaTensor(float mass);

#endif
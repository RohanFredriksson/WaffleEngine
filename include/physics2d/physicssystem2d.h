#include "external.h"
#include "forceregistry.h"

#ifndef PHYSICSSYSTEM2D_H
#define PHYSICSSYSTEM2D_H

struct PhysicsSystem2D {
    
    ForceRegistry forceRegistry;
    Rigidbody2D** rigidbodies;
    int numRigidbodies;
    int sizeRigidbodies;
    ForceGenerator* gravity;
    float fixedUpdate;

};
typedef struct PhysicsSystem2D PhysicsSystem2D;

void PhysicsSystem2D_Init(PhysicsSystem2D* p, float fixedUpdate, vec2 gravity);
void PhysicsSystem2D_Update(PhysicsSystem2D* p, float dt);
void PhysicsSystem2D_FixedUpdate(PhysicsSystem2D* p);
void PhysicsSystem2D_AddRigidbody(PhysicsSystem2D* p, Rigidbody2D* rb);
void PhysicsSystem2D_Free(PhysicsSystem2D* p);

#endif
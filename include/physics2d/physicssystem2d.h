#include "external.h"
#include "gameobject.h"
#include "forceregistry.h"
#include "collisionmanifold.h"

#ifndef PHYSICSSYSTEM2D_H
#define PHYSICSSYSTEM2D_H

struct PhysicsSystem2D {
    
    ForceRegistry forceRegistry;
    ForceGenerator* gravity;

    Rigidbody2D** rigidbodies;
    int numRigidbodies;
    int sizeRigidbodies;
    
    Rigidbody2D** bodies1;
    int numBodies1;
    int sizeBodies1;

    Rigidbody2D** bodies2;
    int numBodies2;
    int sizeBodies2;

    CollisionManifold** collisions;
    int numCollisions;
    int sizeCollisions;

    float fixedUpdate;

};
typedef struct PhysicsSystem2D PhysicsSystem2D;

void PhysicsSystem2D_Init(PhysicsSystem2D* p, float fixedUpdate, vec2 gravity);
void PhysicsSystem2D_Update(PhysicsSystem2D* p, float dt);
void PhysicsSystem2D_FixedUpdate(PhysicsSystem2D* p);
void PhysicsSystem2D_AddRigidbody(PhysicsSystem2D* p, Rigidbody2D* rb);
void PhysicsSystem2D_AddGameObject(PhysicsSystem2D* p, GameObject* go);
void PhysicsSystem2D_Free(PhysicsSystem2D* p);

#endif
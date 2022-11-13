#include <stdbool.h>
#include "external.h"
#include "raycast.h"
#include "gameobject.h"
#include "forceregistry.h"
#include "collisionmanifold.h"

#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

struct PhysicsSystem {
    
    ForceRegistry forceRegistry;
    ForceGenerator* gravity;

    Rigidbody** rigidbodies;
    int numRigidbodies;
    int sizeRigidbodies;
    
    Rigidbody** bodies1;
    int numBodies1;
    int sizeBodies1;

    Rigidbody** bodies2;
    int numBodies2;
    int sizeBodies2;

    CollisionManifold** collisions;
    int numCollisions;
    int sizeCollisions;

    float fixedUpdate;

};
typedef struct PhysicsSystem PhysicsSystem;

void PhysicsSystem_Init(PhysicsSystem* p, float fixedUpdate, vec2 gravity);
void PhysicsSystem_Update(PhysicsSystem* p, float dt);
void PhysicsSystem_Render(PhysicsSystem* p);
void PhysicsSystem_FixedUpdate(PhysicsSystem* p);
void PhysicsSystem_AddRigidbody(PhysicsSystem* p, Rigidbody* rb);
void PhysicsSystem_AddGameObject(PhysicsSystem* p, GameObject* go);
bool PhysicsSystem_Raycast(PhysicsSystem*p, Ray ray, RaycastResult* result);
void PhysicsSystem_Free(PhysicsSystem* p);

#endif
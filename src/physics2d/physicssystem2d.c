#include <stdlib.h>
#include <string.h>
#include "physicssystem2d.h"
#include "gravity2d.h"

#define INITIAL_RIGIDBODIES_SIZE 16

void PhysicsSystem2D_Init(PhysicsSystem2D* p, float fixedUpdate, vec2 gravity) {

    ForceRegistry_Init(&p->forceRegistry);
    p->rigidbodies = malloc(INITIAL_RIGIDBODIES_SIZE * sizeof(Rigidbody2D*));
    p->sizeRigidbodies = INITIAL_RIGIDBODIES_SIZE;
    p->numRigidbodies = 0;
    p->fixedUpdate = fixedUpdate;
    p->gravity = Gravity2D_Init(gravity);

}

void PhysicsSystem2D_Update(PhysicsSystem2D* p, float dt) {
    PhysicsSystem2D_FixedUpdate(p);
}

void PhysicsSystem2D_FixedUpdate(PhysicsSystem2D* p) {
    
    ForceRegistry_UpdateForces(&p->forceRegistry, p->fixedUpdate);

    // Update velocities of all rigidbodies
    for (int i = 0; i < p->numRigidbodies; i++) {
        Rigidbody2D_PhysicsUpdate(p->rigidbodies[i], p->fixedUpdate);
    }

}

void PhysicsSystem2D_AddRigidbody(PhysicsSystem2D* p, Rigidbody2D* rb) {

    if (p->numRigidbodies >= p->sizeRigidbodies) {
        p->rigidbodies = realloc(p->rigidbodies, p->sizeRigidbodies * 2 * sizeof(Rigidbody2D*));
        p->sizeRigidbodies = p->sizeRigidbodies * 2;
    }

    p->rigidbodies[p->numRigidbodies] = rb;
    p->numRigidbodies++;

    // Register gravity
    ForceRegistry_Add(&p->forceRegistry, p->gravity, rb);

}

void PhysicsSystem2D_AddGameObject(PhysicsSystem2D* p, GameObject* go) {

    for (int i = 0; i < go->numComponents; i++) {
        if (strcmp(go->components[i]->type, "Rigidbody2D") == 0) {
            PhysicsSystem2D_AddRigidbody(p, (Rigidbody2D*) go->components[i]->data);
        }
    }

}

void PhysicsSystem2D_Free(PhysicsSystem2D* p) {
    ForceRegistry_Free(&p->forceRegistry);
    free(p->rigidbodies);
    ForceGenerator_Free(p->gravity);
    free(p->gravity);
}
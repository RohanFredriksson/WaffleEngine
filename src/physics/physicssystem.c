#include <stdlib.h>
#include <string.h>
#include "physicssystem.h"
#include "primitives.h"
#include "debugdraw.h"
#include "collision.h"
#include "gravity.h"
#include "wmath.h"

#define INITIAL_RIGIDBODIES_SIZE 16
#define INITIAL_BODIES_SIZE 16
#define INITIAL_COLLISIONS_SIZE 16
#define IMPULSE_ITERATIONS 6

void PhysicsSystem_Init(PhysicsSystem* p, float fixedUpdate, vec2 gravity) {

    ForceRegistry_Init(&p->forceRegistry);
    p->gravity = Gravity_Init(gravity);
    
    p->rigidbodies = malloc(INITIAL_RIGIDBODIES_SIZE * sizeof(Rigidbody*));
    p->sizeRigidbodies = INITIAL_RIGIDBODIES_SIZE;
    p->numRigidbodies = 0;

    p->bodies1 = malloc(INITIAL_BODIES_SIZE * sizeof(Rigidbody*));
    p->sizeBodies1 = INITIAL_BODIES_SIZE;
    p->numBodies1 = 0;

    p->bodies2 = malloc(INITIAL_BODIES_SIZE * sizeof(Rigidbody*));
    p->sizeBodies2 = INITIAL_BODIES_SIZE;
    p->numBodies2 = 0;

    p->collisions = malloc(INITIAL_COLLISIONS_SIZE * sizeof(CollisionManifold*));
    p->sizeCollisions = INITIAL_COLLISIONS_SIZE;
    p->numCollisions = 0;

    p->fixedUpdate = fixedUpdate;

}

void PhysicsSystem_Update(PhysicsSystem* p, float dt) {
    PhysicsSystem_FixedUpdate(p);
}

void PhysicsSystem_Render(PhysicsSystem* p) {
    for (int i = 0 ; i < p->numRigidbodies; i++) {

        Rigidbody* rigidbody = p->rigidbodies[i];
        Collider* collider = rigidbody->collider;

        if (collider == NULL) {continue;}

        if (strcmp(collider->type, "Circle") == 0) {
            Circle* circle = (Circle*) collider->data;
            DebugDraw_AddCircle(rigidbody->transform->pos, circle->radius, (vec3) { 0.0f, 1.0f, 0.0f }, 1);
        } else if (strcmp(collider->type, "Box") == 0) {
            Box* box = (Box*) collider->data;
            DebugDraw_AddBox(rigidbody->transform->pos, box->size, rigidbody->transform->rotation, (vec3) {0.0f, 1.0f, 0.0f }, 1);
        }

    }
}

void PhysicsSystem_ClearCollisionLists(PhysicsSystem* p) {

    p->numBodies1 = 0;
    p->numBodies2 = 0;
    p->numCollisions = 0;
    for (int i = 0; i < p->numCollisions; i++) {
        CollisionManifold_Free(p->collisions[i]);
        free(p->collisions[i]);
    }

}

void PhysicsSystem_ApplyImpulse(Rigidbody* a, Rigidbody* b, CollisionManifold* m) {

    // Linear velocity
    float invMass1 = 1.0f / a->mass;
    float invMass2 = 1.0f / b->mass;
    float invMassSum = invMass1 + invMass2;
    if (invMassSum == 0.0f) {return;}

    // Relative velocity
    vec2 relativeVelocity;
    glm_vec2_sub(b->linearVelocity, a->linearVelocity, relativeVelocity);
    vec2 relativeNormal;
    glm_vec2_normalize_to(m->normal, relativeNormal);

    // Moving away from each other? Do nothing.
    if (glm_vec2_dot(relativeVelocity, relativeNormal) > 0.0f) {return;}

    float e = WMath_MinFloat(a->cor, b->cor);
    float numerator = -(1.0f + e) * glm_vec2_dot(relativeVelocity, relativeNormal);
    float j = numerator / invMassSum;

    if (m->numContactPoints > 0 && j != 0.0f) {
        j = j / (float) m->numContactPoints;
    }

    vec2 impulse;
    glm_vec2_scale(relativeNormal, j, impulse);

    vec2 atmp;
    glm_vec2_scale(impulse, -1.0f * invMass1, atmp);
    glm_vec2_add(a->linearVelocity, atmp, a->linearVelocity);

    vec2 btmp;
    glm_vec2_scale(impulse, invMass2, btmp);
    glm_vec2_add(b->linearVelocity, btmp, b->linearVelocity);

}

void PhysicsSystem_FixedUpdate(PhysicsSystem* p) {
    
    // Clear the collision lists
    PhysicsSystem_ClearCollisionLists(p);

    // Find any collisions
    int size = p->numRigidbodies;
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) {continue;}

            Rigidbody* r1 = p->rigidbodies[i];
            Rigidbody* r2 = p->rigidbodies[j];

            Collider* c1 = r1->collider;
            Collider* c2 = r2->collider;

            CollisionManifold* result = NULL;
            if (c1 != NULL && c2 != NULL && !(Rigidbody_HasInfiniteMass(r1) && Rigidbody_HasInfiniteMass(r2))) {
                result = Collision_FindCollisionFeatures(c1, c2);
            }

            if (result != NULL && result->isColliding) {

                if (p->numBodies1 >= p->sizeBodies1) {
                    p->bodies1 = realloc(p->bodies1, 2 * p->sizeBodies1 * sizeof(Rigidbody*));
                    p->sizeBodies1 = p->sizeBodies1 * 2;
                }
                p->bodies1[p->numBodies1] = r1;
                p->numBodies1++;

                if (p->numBodies2 >= p->sizeBodies2) {
                    p->bodies2 = realloc(p->bodies2, 2 * p->sizeBodies2 * sizeof(Rigidbody*));
                    p->sizeBodies2 = p->sizeBodies2 * 2;
                }
                p->bodies2[p->numBodies2] = r2;
                p->numBodies2++;

                if (p->numCollisions >= p->sizeCollisions) {
                    p->collisions = realloc(p->collisions, 2 * p->sizeCollisions * sizeof(CollisionManifold*));
                    p->sizeCollisions = p->sizeCollisions * 2;
                }
                p->collisions[p->numCollisions] = result;
                p->numCollisions++;

            } else if (result != NULL) {
                CollisionManifold_Free(result);
                free(result);
            }
        }
    }

    // Update the forces
    ForceRegistry_UpdateForces(&p->forceRegistry, p->fixedUpdate);

    // Resolve collisions via iterative impulse resolution
    for (int k = 0; k < IMPULSE_ITERATIONS; k++) {
        for (int i = 0; i < p->numCollisions; i++) {
            int jSize = p->collisions[i]->numContactPoints;
            for (int j=0; j < jSize; j++) {
                Rigidbody* r1 = p->bodies1[i];
                Rigidbody* r2 = p->bodies2[i];
                PhysicsSystem_ApplyImpulse(r1, r2, p->collisions[i]);
            }
        }
    }

    // Update velocities of all rigidbodies
    for (int i = 0; i < p->numRigidbodies; i++) {
        Rigidbody_PhysicsUpdate(p->rigidbodies[i], p->fixedUpdate);
    }

}

void PhysicsSystem_AddRigidbody(PhysicsSystem* p, Rigidbody* rb) {

    if (p->numRigidbodies >= p->sizeRigidbodies) {
        p->rigidbodies = realloc(p->rigidbodies, p->sizeRigidbodies * 2 * sizeof(Rigidbody*));
        p->sizeRigidbodies = p->sizeRigidbodies * 2;
    }

    p->rigidbodies[p->numRigidbodies] = rb;
    p->numRigidbodies++;

    // Register gravity
    ForceRegistry_Add(&p->forceRegistry, p->gravity, rb);

}

void PhysicsSystem_AddGameObject(PhysicsSystem* p, GameObject* go) {

    for (int i = 0; i < go->numComponents; i++) {
        if (strcmp(go->components[i]->type, "Rigidbody") == 0) {
            PhysicsSystem_AddRigidbody(p, (Rigidbody*) go->components[i]->data);
        }
    }

}

void PhysicsSystem_Free(PhysicsSystem* p) {
    
    ForceRegistry_Free(&p->forceRegistry);
    ForceGenerator_Free(p->gravity);
    free(p->gravity);
    
    free(p->rigidbodies);
    free(p->bodies1);
    free(p->bodies2);

    for (int i = 0; i < p->numCollisions; i++) {
        CollisionManifold_Free(p->collisions[i]);
        free(p->collisions[i]);
    }
    free(p->collisions);

}
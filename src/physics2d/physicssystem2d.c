#include <stdlib.h>
#include <string.h>
#include "physicssystem2d.h"
#include "debugdraw.h"
#include "collision.h"
#include "gravity2d.h"
#include "wmath.h"

#define INITIAL_RIGIDBODIES_SIZE 16
#define INITIAL_BODIES_SIZE 16
#define INITIAL_COLLISIONS_SIZE 16
#define IMPULSE_ITERATIONS 6

void PhysicsSystem2D_Init(PhysicsSystem2D* p, float fixedUpdate, vec2 gravity) {

    ForceRegistry_Init(&p->forceRegistry);
    p->gravity = Gravity2D_Init(gravity);
    
    p->rigidbodies = malloc(INITIAL_RIGIDBODIES_SIZE * sizeof(Rigidbody2D*));
    p->sizeRigidbodies = INITIAL_RIGIDBODIES_SIZE;
    p->numRigidbodies = 0;

    p->bodies1 = malloc(INITIAL_BODIES_SIZE * sizeof(Rigidbody2D*));
    p->sizeBodies1 = INITIAL_BODIES_SIZE;
    p->numBodies1 = 0;

    p->bodies2 = malloc(INITIAL_BODIES_SIZE * sizeof(Rigidbody2D*));
    p->sizeBodies2 = INITIAL_BODIES_SIZE;
    p->numBodies2 = 0;

    p->collisions = malloc(INITIAL_COLLISIONS_SIZE * sizeof(CollisionManifold*));
    p->sizeCollisions = INITIAL_COLLISIONS_SIZE;
    p->numCollisions = 0;

    p->fixedUpdate = fixedUpdate;

}

void PhysicsSystem2D_Update(PhysicsSystem2D* p, float dt) {
    PhysicsSystem2D_FixedUpdate(p);
}

void PhysicsSystem2D_Render(PhysicsSystem2D* p) {
    for (int i = 0 ; i < p->numRigidbodies; i++) {

        Rigidbody2D* rigidbody = p->rigidbodies[i];
        Collider2D* collider = rigidbody->collider;

        if (collider == NULL) {continue;}

        if (strcmp(collider->type, "Circle") == 0) {
            Circle* circle = (Circle*) collider->data;
            DebugDraw_AddCircle(rigidbody->transform->pos, circle->radius, (vec3) { 0.0f, 1.0f, 0.0f }, 1);
        }

    }
}

void PhysicsSystem2D_ClearCollisionLists(PhysicsSystem2D* p) {

    p->numBodies1 = 0;
    p->numBodies2 = 0;
    p->numCollisions = 0;
    for (int i = 0; i < p->numCollisions; i++) {
        CollisionManifold_Free(p->collisions[i]);
        free(p->collisions[i]);
    }

}

void PhysicsSystem2D_ApplyImpulse(Rigidbody2D* a, Rigidbody2D* b, CollisionManifold* m) {

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

void PhysicsSystem2D_FixedUpdate(PhysicsSystem2D* p) {
    
    // Clear the collision lists
    PhysicsSystem2D_ClearCollisionLists(p);

    // Find any collisions
    int size = p->numRigidbodies;
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) {continue;}

            Rigidbody2D* r1 = p->rigidbodies[i];
            Rigidbody2D* r2 = p->rigidbodies[j];

            Collider2D* c1 = r1->collider;
            Collider2D* c2 = r2->collider;

            CollisionManifold* result = NULL;
            if (c1 != NULL && c2 != NULL && !(Rigidbody2D_HasInfiniteMass(r1) && Rigidbody2D_HasInfiniteMass(r2))) {
                result = Collision_FindCollisionFeatures(c1, c2);
            }

            if (result != NULL && result->isColliding) {

                if (p->numBodies1 >= p->sizeBodies1) {
                    p->bodies1 = realloc(p->bodies1, 2 * p->sizeBodies1 * sizeof(Rigidbody2D*));
                    p->sizeBodies1 = p->sizeBodies1 * 2;
                }
                p->bodies1[p->numBodies1] = r1;
                p->numBodies1++;

                if (p->numBodies2 >= p->sizeBodies2) {
                    p->bodies2 = realloc(p->bodies2, 2 * p->sizeBodies2 * sizeof(Rigidbody2D*));
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
                Rigidbody2D* r1 = p->bodies1[i];
                Rigidbody2D* r2 = p->bodies2[i];
                PhysicsSystem2D_ApplyImpulse(r1, r2, p->collisions[i]);
            }
        }
    }

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
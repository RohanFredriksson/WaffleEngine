#include <stdlib.h>
#include <string.h>
#include "physicssystem.h"
#include "primitives.h"
#include "debugdraw.h"
#include "collision.h"
#include "gravity.h"
#include "wmath.h"
#include "list.h"

#define INITIAL_RIGIDBODIES_SIZE 16
#define INITIAL_BODIES_SIZE 16
#define INITIAL_COLLISIONS_SIZE 16
#define IMPULSE_ITERATIONS 6

float PhysicsSystem_Time = 0.0f;

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
    PhysicsSystem_Time = PhysicsSystem_Time + dt;
    while (PhysicsSystem_Time >= 0.0f) {
        PhysicsSystem_FixedUpdate(p);
        PhysicsSystem_Time = PhysicsSystem_Time - p->fixedUpdate;
    }
}

void PhysicsSystem_Render(PhysicsSystem* p) {
    for (int i = 0 ; i < p->numRigidbodies; i++) {

        Rigidbody* rigidbody = p->rigidbodies[i];
        Collider* collider = (Collider*) Rigidbody_GetCollider(rigidbody)->data;

        if (collider == NULL) {continue;}

        vec2 position;
        Component_GetPosition(rigidbody->component, position);
        if (strcmp(collider->type, "Circle") == 0) {
            Circle* circle = (Circle*) collider->data;
            DebugDraw_AddCircle(position, circle->radius, (vec3) { 0.0f, 1.0f, 0.0f }, 1);
        } else if (strcmp(collider->type, "Box") == 0) {
            Box* box = (Box*) collider->data;
            DebugDraw_AddBox(position, box->size, /*Component_GetRotation(rigidbody->component)*/ 0.0f, (vec3) {0.0f, 1.0f, 0.0f }, 1);
        }

    }
}

void PhysicsSystem_ClearCollisionLists(PhysicsSystem* p) {

    for (int i = 0; i < p->numCollisions; i++) {
        free(p->collisions[i]);
    }

    p->numBodies1 = 0;
    p->numBodies2 = 0;
    p->numCollisions = 0;

}

void PhysicsSystem_ApplyImpulse(Rigidbody* a, Rigidbody* b, CollisionManifold* m) {

    // Check for infinite mass objects.
    if (Rigidbody_HasInfiniteMass(a) && Rigidbody_HasInfiniteMass(b)) {return;}

    // If one rigidbody is static.
    if (Rigidbody_HasInfiniteMass(a) || Rigidbody_HasInfiniteMass(b)) {
        
        // Relative velocity
        vec2 relativeVelocity;
        glm_vec2_sub(b->velocity, a->velocity, relativeVelocity);
        vec2 relativeNormal;
        glm_vec2_normalize_to(m->normal, relativeNormal);

        // Moving away from each other? Do nothing.
        if (glm_vec2_dot(relativeVelocity, relativeNormal) > 0.0f) {return;}

        float e = WMath_MinFloat(a->cor, b->cor);
        float numerator = -(1.0f + e) * glm_vec2_dot(relativeVelocity, relativeNormal);
        float j = numerator / ((Rigidbody_HasInfiniteMass(a)) ? 1.0f / b->mass : 1.0f / a->mass);

        vec2 impulse;
        glm_vec2_scale(relativeNormal, j, impulse);

        if (Rigidbody_HasInfiniteMass(a)) {
            
            vec2 btmp;
            glm_vec2_scale(impulse, 1.0f / b->mass, btmp);
            glm_vec2_add(b->velocity, btmp, b->velocity);

            // For static objects, move them outside of the obstacles, so they dont eventually sink through.
            glm_vec2_scale(m->normal, 2.0f * m->depth, btmp);
            glm_vec2_add(b->component->entity->position, btmp, b->component->entity->position);

        }

        else {

            vec2 atmp;
            glm_vec2_scale(impulse, -1.0f / a->mass, atmp);
            glm_vec2_add(a->velocity, atmp, a->velocity);

            // For static objects, move them outside of the obstacles, so they dont eventually sink through.
            glm_vec2_scale(m->normal, -2.0f * m->depth, atmp);
            glm_vec2_add(a->component->entity->position, atmp, a->component->entity->position);

        }

    }

    else {

        // Linear velocity
        float invMass1 = 1.0f / a->mass;
        float invMass2 = 1.0f / b->mass;
        float invMassSum = invMass1 + invMass2;
        if (invMassSum == 0.0f) {return;}

        // Relative velocity
        vec2 relativeVelocity;
        glm_vec2_sub(b->velocity, a->velocity, relativeVelocity);
        vec2 relativeNormal;
        glm_vec2_normalize_to(m->normal, relativeNormal);

        // Moving away from each other? Do nothing.
        if (glm_vec2_dot(relativeVelocity, relativeNormal) > 0.0f) {return;}

        float e = WMath_MinFloat(a->cor, b->cor);
        float numerator = -(1.0f + e) * glm_vec2_dot(relativeVelocity, relativeNormal);
        float j = numerator / invMassSum;

        vec2 impulse;
        glm_vec2_scale(relativeNormal, j, impulse);

        vec2 atmp;
        glm_vec2_scale(impulse, -1.0f * invMass1, atmp);
        glm_vec2_add(a->velocity, atmp, a->velocity);

        vec2 btmp;
        glm_vec2_scale(impulse, invMass2, btmp);
        glm_vec2_add(b->velocity, btmp, b->velocity);

    }

}

void PhysicsSystem_FixedUpdate(PhysicsSystem* p) {
    
    // Update velocities of all rigidbodies
    for (int i = 0; i < p->numRigidbodies; i++) {
        Rigidbody_PhysicsUpdate(p->rigidbodies[i], p->fixedUpdate);
    }

    // Clear the collision lists
    PhysicsSystem_ClearCollisionLists(p);

    // Find any collisions
    // TODO: IMPROVE THIS WITH SPATIAL HASHING
    int size = p->numRigidbodies;
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) {continue;}

            Rigidbody* r1 = p->rigidbodies[i];
            Rigidbody* r2 = p->rigidbodies[j];

            Collider* c1 = (Collider*) Rigidbody_GetCollider(r1)->data;
            Collider* c2 = (Collider*) Rigidbody_GetCollider(r2)->data;

            CollisionManifold* result = NULL;
            if (c1 != NULL && c2 != NULL && !(Rigidbody_HasInfiniteMass(r1) && Rigidbody_HasInfiniteMass(r2))) {
                result = Collision_FindCollisionFeatures(c1, c2);
            }

            if (result != NULL && result->isColliding) {

                // Execute on collision
                Entity_OnCollision(c1->component->entity, c2->component->entity, result->contactPoint, result->normal);
                Entity_OnCollision(c2->component->entity, c1->component->entity, result->contactPoint, result->normal);

                // If neither object is a trigger, do the collision.
                if (!r1->sensor && !r2->sensor) {

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

                } else {
                    free(result);
                }

            } else if (result != NULL) {
                free(result);
            }
        }
    }

    // Update the forces
    ForceRegistry_UpdateForces(&p->forceRegistry, p->fixedUpdate);

    // Resolve collisions via iterative impulse resolution
    for (int k = 0; k < IMPULSE_ITERATIONS; k++) {
        for (int i = 0; i < p->numCollisions; i++) {
            Rigidbody* r1 = p->bodies1[i];
            Rigidbody* r2 = p->bodies2[i];
            PhysicsSystem_ApplyImpulse(r1, r2, p->collisions[i]);
        }
    }

}

static void PhysicsSystem_AddRigidbody(PhysicsSystem* p, Rigidbody* rb) {

    if (p->numRigidbodies >= p->sizeRigidbodies) {
        p->rigidbodies = realloc(p->rigidbodies, p->sizeRigidbodies * 2 * sizeof(Rigidbody*));
        p->sizeRigidbodies = p->sizeRigidbodies * 2;
    }

    p->rigidbodies[p->numRigidbodies] = rb;
    p->numRigidbodies++;

    // Register gravity
    ForceRegistry_Add(&p->forceRegistry, p->gravity, rb);

}

static void PhysicsSystem_RemoveRigidbody(PhysicsSystem* p, Rigidbody* rb) {

    for (int i = 0; i < p->numRigidbodies; i++) {
        Rigidbody* current = p->rigidbodies[i];
        if (current == rb) {
            ForceRegistry_Remove(&p->forceRegistry, p->gravity, current);
            memmove(current, current + 1, (p->numRigidbodies - i - 1) * sizeof(Rigidbody*));
            p->numRigidbodies--;
            return;
        }
    }

}

void PhysicsSystem_AddComponent(PhysicsSystem* p, Component* c) {
    if (strcmp(c->type, "Rigidbody") == 0) {
        PhysicsSystem_AddRigidbody(p, (Rigidbody*) c->data);
    }
}

void PhysicsSystem_RemoveComponent(PhysicsSystem* p, Component* c) {
    if (strcmp(c->type, "Rigidbody") == 0) {
        PhysicsSystem_RemoveRigidbody(p, (Rigidbody*) c->data);
    }
}

bool PhysicsSystem_Raycast(PhysicsSystem*p, Ray ray, RaycastResult* result) {

    bool hit = 0;
    RaycastResult best;
    RaycastResult current;
    best.t = FLT_MAX;

    // TODO: IMPROVE THIS WITH SPATIAL HASHING
    for (int i = 0; i < p->numRigidbodies; i++) {

        if (Raycast_Rigidbody(p->rigidbodies[i], ray, &current)) {
            if (current.t < best.t) {
                best = current;
                hit = 1;
            }
        }

    }

    if (hit) {
        memcpy(result, &best, sizeof(RaycastResult));
    }

    return hit;

}

void PhysicsSystem_Free(PhysicsSystem* p) {
    
    ForceRegistry_Free(&p->forceRegistry);
    ForceGenerator_Free(p->gravity);
    free(p->gravity);
    
    free(p->rigidbodies);
    free(p->bodies1);
    free(p->bodies2);

    for (int i = 0; i < p->numCollisions; i++) {
        free(p->collisions[i]);
    }
    free(p->collisions);

}
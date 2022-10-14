#include "stdlib.h"
#include "string.h"
#include "external.h"
#include "physics.h"
#include "rigidbody.h"

#define INITIAL_BODIES_SIZE 16

void Physics_Init(Physics* p, vec2 gravity) {
    
    glm_vec2_copy(gravity, p->gravity);
    p->bodies = (RigidBody**) malloc(INITIAL_BODIES_SIZE * sizeof(RigidBody*));
    p->numBodies = 0;
    p->sizeBodies = INITIAL_BODIES_SIZE;

}

void Physics_Update(Physics* p, float dt) {
    
    for (int i = 0; i < p->numBodies; i++) {
        
        RigidBody* currentBody = p->bodies[i];

        // If the body is not static, go to next object.
        if (currentBody->bodyType == BODYTYPE_STATIC) {
            continue;
        }
        
        // Update the position of the body.
        vec2 displacement;
        glm_vec2_scale(currentBody->velocity, dt, displacement);
        glm_vec2_add(currentBody->transform->pos, displacement, currentBody->transform->pos);

        // Apply gravity.
        vec2 velocity;
        glm_vec2_scale(p->gravity, dt, velocity);
        glm_vec2_add(currentBody->velocity, velocity, currentBody->velocity);

        // Check for collisions.
        for (int j = 0; j < p->numBodies; j++) {

        }

    }

}

void Physics_Free(Physics* p) {
    free(p->bodies);
}

void Physics_AddGameObject(Physics* p, GameObject* go) {
    
    for (int i = 0; i < go->numComponents; i++) {
        if (strcmp(go->components[i]->type, "RigidBody") == 0) {
            Physics_AddRigidBody(p, (RigidBody*) go->components[i]->data);
        }
    }

}

void Physics_AddRigidBody(Physics* p, RigidBody* rb) {

    if (p->numBodies >= p->sizeBodies) {
        p->bodies = (RigidBody**) realloc(p->bodies, p->sizeBodies * 2 * sizeof(RigidBody*));
        p->sizeBodies = p->sizeBodies * 2;
    }

    p->bodies[p->numBodies] = rb;
    p->numBodies++;

}

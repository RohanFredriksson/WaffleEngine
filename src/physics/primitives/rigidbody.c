#include <string.h>
#include <stdio.h>
#include "rigidbody.h"

Component* Rigidbody_Init(Transform* transform) {

    Component* c = Component_Init("Rigidbody", &Rigidbody_Update, &Rigidbody_Free);

    // Allocate some memory for the rigidbody.
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    // Initialise the rigidbody.
    rb->component = c;
    rb->collider = NULL;
    rb->transform = transform;
    rb->mass = 1.0f;
    glm_vec2_zero(rb->forceAccum);
    glm_vec2_zero(rb->linearVelocity);
    rb->angularVelocity = 0.0f;
    rb->linearDamping = 0.0f;
    rb->angularDamping = 0.0f;
    rb->cor = 1.0f;
    rb->fixedRotation = 0;

    // Attach the rigidbody to the component
    c->data = rb;
    return c;

}

void Rigidbody_Update(Component* c, float dt) {
    
}

void Rigidbody_Free(Component* c) {
    
}

void Rigidbody_SetCollider(Component* c, Component* collider) {
    
    if (strcmp(c->type, "Rigidbody") != 0) {
        printf("ERROR::RIGIDBODY_SETCOLLIDER::COMPONENT_NOT_OF_RIGIDBODY_TYPE\n");
        return;
    }

    if (strcmp(collider->type, "Collider") != 0) {
        printf("ERROR::RIGIDBODY_SETCOLLIDER::COMPONENT_NOT_OF_COLLIDER_TYPE");
        return;
    }

    ((Rigidbody*) c->data)->collider = (Collider*) collider->data;
}

void Rigidbody_ClearAccumulators(Rigidbody* rb) {
    glm_vec2_zero(rb->forceAccum);
}

void Rigidbody_PhysicsUpdate(Rigidbody* rb, float dt) {
    
    if (rb->mass == 0.0f) {return;}

    // Calculate linear velocity
    vec2 dv;
    glm_vec2_scale(rb->forceAccum, dt/rb->mass, dv);
    glm_vec2_add(rb->linearVelocity, dv, rb->linearVelocity);

    // Calculate linear position
    vec2 ds;
    glm_vec2_scale(rb->linearVelocity, dt, ds);
    glm_vec2_add(rb->transform->pos, ds, rb->transform->pos);
    Rigidbody_ClearAccumulators(rb);

}

void Rigidbody_AddForce(Rigidbody* rb, vec2 force) {
    glm_vec2_add(rb->forceAccum, force, rb->forceAccum);
}

bool Rigidbody_HasInfiniteMass(Rigidbody* rb) {
    return rb->mass == 0.0f;
}
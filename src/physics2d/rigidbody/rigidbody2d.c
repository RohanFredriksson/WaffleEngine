#include <string.h>
#include <stdio.h>
#include "rigidbody2d.h"

Component* Rigidbody2D_Init(Transform* transform) {

    Component* c = Component_Init("Rigidbody2D", &Rigidbody2D_Update, &Rigidbody2D_Free);

    // Allocate some memory for the rigidbody.
    Rigidbody2D* rb = malloc(sizeof(Rigidbody2D));

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

void Rigidbody2D_Update(Component* c, float dt) {
    
}

void Rigidbody2D_Free(Component* c) {
    
}

void Rigidbody2D_SetCollider(Component* c, Component* collider) {
    
    if (strcmp(c->type, "Rigidbody2D") != 0) {
        printf("ERROR::RIGIDBODY2D_SETCOLLIDER::COMPONENT_NOT_OF_RIGIDBODY2D_TYPE\n");
        return;
    }

    if (strcmp(collider->type, "Collider2D") != 0) {
        printf("ERROR::RIGIDBODY2D_SETCOLLIDER::COMPONENT_NOT_OF_COLLIDER2D_TYPE");
        return;
    }

    ((Rigidbody2D*) c->data)->collider = (Collider2D*) collider->data;
}

void Rigidbody2D_ClearAccumulators(Rigidbody2D* rb) {
    glm_vec2_zero(rb->forceAccum);
}

void Rigidbody2D_PhysicsUpdate(Rigidbody2D* rb, float dt) {
    
    if (rb->mass == 0.0f) {return;}

    // Calculate linear velocity
    vec2 dv;
    glm_vec2_scale(rb->forceAccum, dt/rb->mass, dv);
    glm_vec2_add(rb->linearVelocity, dv, rb->linearVelocity);

    // Calculate linear position
    vec2 ds;
    glm_vec2_scale(rb->linearVelocity, dt, ds);
    glm_vec2_add(rb->transform->pos, ds, rb->transform->pos);
    Rigidbody2D_ClearAccumulators(rb);

}

void Rigidbody2D_AddForce(Rigidbody2D* rb, vec2 force) {
    glm_vec2_add(rb->forceAccum, force, rb->forceAccum);
}

bool Rigidbody2D_HasInfiniteMass(Rigidbody2D* rb) {
    return rb->mass == 0.0f;
}
#include <string.h>
#include <stdio.h>
#include "rigidbody.h"

Component* Rigidbody_Init() {

    Component* c = Component_Init("Rigidbody", NULL, NULL, NULL);

    // Allocate some memory for the rigidbody.
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    // Initialise the rigidbody.
    rb->component = c;
    rb->collider = NULL;
    rb->mass = 1.0f;
    glm_vec2_zero(rb->forceAccum);
    glm_vec2_zero(rb->velocity);
    rb->sensor = 0;
    rb->cor = 1.0f;

    // Attach the rigidbody to the component
    c->data = rb;
    return c;

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

void Rigidbody_SetMass(Component* c, float mass) {

    if (strcmp(c->type, "Rigidbody") != 0) {
        printf("ERROR::RIGIDBODY_SETMASS::COMPONENT_NOT_OF_RIGIDBODY_TYPE\n");
        return;
    }

    ((Rigidbody*) c->data)->mass = mass;

}

void Rigidbody_SetVelocity(Component* c, vec2 velocity) {
    
    if (strcmp(c->type, "Rigidbody") != 0) {
        printf("ERROR::RIGIDBODY_SETVELOCITY::COMPONENT_NOT_OF_RIGIDBODY_TYPE\n");
        return;
    }

    glm_vec2_copy(velocity, ((Rigidbody*) c->data)->velocity);

}

void Rigidbody_SetSensor(Component* c, bool sensor) {
    
    if (strcmp(c->type, "Rigidbody") != 0) {
        printf("ERROR::RIGIDBODY_SETSENSOR::COMPONENT_NOT_OF_RIGIDBODY_TYPE\n");
        return;
    }

    ((Rigidbody*) c->data)->sensor = sensor;

}

void Rigidbody_SetCor(Component* c, float cor) {
    
    if (strcmp(c->type, "Rigidbody") != 0) {
        printf("ERROR::RIGIDBODY_SETCOR::COMPONENT_NOT_OF_RIGIDBODY_TYPE\n");
        return;
    }

    ((Rigidbody*) c->data)->cor = cor;

}

void Rigidbody_ClearAccumulators(Rigidbody* rb) {
    glm_vec2_zero(rb->forceAccum);
}

void Rigidbody_PhysicsUpdate(Rigidbody* rb, float dt) {
    
    if (rb->mass == 0.0f) {return;}

    // Calculate linear velocity
    vec2 dv;
    glm_vec2_scale(rb->forceAccum, dt/rb->mass, dv);
    glm_vec2_add(rb->velocity, dv, rb->velocity);

    // Calculate linear position
    vec2 ds;
    glm_vec2_scale(rb->velocity, dt, ds);
    glm_vec2_add(rb->component->entity->position, ds, rb->component->entity->position);
    Rigidbody_ClearAccumulators(rb);

}

void Rigidbody_AddForce(Rigidbody* rb, vec2 force) {
    glm_vec2_add(rb->forceAccum, force, rb->forceAccum);
}

bool Rigidbody_HasInfiniteMass(Rigidbody* rb) {
    return rb->mass == 0.0f;
}
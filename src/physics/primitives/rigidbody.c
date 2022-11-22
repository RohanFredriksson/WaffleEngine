#include "rigidbody.h"

Component* Rigidbody_Init() {

    Component* c = Component_Init("Rigidbody", NULL, NULL, &Rigidbody_Serialise, NULL);

    // Allocate some memory for the rigidbody.
    Rigidbody* rb = malloc(sizeof(Rigidbody));

    // Initialise the rigidbody.
    rb->component = c;
    rb->collider = -1;
    rb->mass = 1.0f;
    glm_vec2_zero(rb->forceAccum);
    glm_vec2_zero(rb->velocity);
    rb->sensor = 0;
    rb->cor = 1.0f;

    // Attach the rigidbody to the component
    c->data = rb;
    return c;

}

cJSON* Rigidbody_Serialise(Component* c) {

    Rigidbody* rb = (Rigidbody*) c->data;

    cJSON* json = cJSON_CreateObject();

    cJSON* collider = cJSON_CreateNumber((double) rb->collider);
    cJSON_AddItemToObject(json, "collider", collider);

    cJSON* mass = cJSON_CreateNumber((double) rb->mass);
    cJSON_AddItemToObject(json, "mass", mass);

    cJSON* forceAccum = cJSON_CreateArray();
    cJSON* xForceAccum = cJSON_CreateNumber((double) rb->forceAccum[0]);
    cJSON* yForceAccum = cJSON_CreateNumber((double) rb->forceAccum[1]);
    cJSON_AddItemToArray(forceAccum, xForceAccum);
    cJSON_AddItemToArray(forceAccum, yForceAccum);
    cJSON_AddItemToObject(json, "forceAccum", forceAccum);

    cJSON* velocity = cJSON_CreateArray();
    cJSON* xVelocity = cJSON_CreateNumber((double) rb->velocity[0]);
    cJSON* yVelocity = cJSON_CreateNumber((double) rb->velocity[1]);
    cJSON_AddItemToArray(velocity, xVelocity);
    cJSON_AddItemToArray(velocity, yVelocity);
    cJSON_AddItemToObject(json, "velocity", velocity);

    cJSON* sensor = cJSON_CreateBool(rb->sensor);
    cJSON_AddItemToObject(json, "sensor", sensor);

    cJSON* cor = cJSON_CreateNumber((double) rb->cor);
    cJSON_AddItemToObject(json, "cor", cor);

    return json;

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

    ((Rigidbody*) c->data)->collider = collider->id;
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

Component* Rigidbody_GetCollider(Rigidbody* rb) {
    return Entity_GetComponentByID(rb->component->entity, rb->collider);
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
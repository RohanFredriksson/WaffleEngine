#include "rigidbody.h"
#include "physicssystem.h"
#include "window.h"
#include "scene.h"

static cJSON* Rigidbody_Serialise(Component* c) {

    Rigidbody* rb = (Rigidbody*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "collider", rb->collider);
    WIO_AddFloat(json, "mass", rb->mass);
    WIO_AddVec2(json, "forceAccum", rb->forceAccum);
    WIO_AddVec2(json, "velocity", rb->velocity);
    WIO_AddBool(json, "sensor", rb->sensor);
    WIO_AddFloat(json, "cor", rb->cor);
    return json;

}

static void Rigidbody_Free(Component* c) {

    Rigidbody* rb = (Rigidbody*) c->data;
    Scene* scene = Window_GetScene();
    PhysicsSystem_RemoveRigidbody(&scene->physics, rb);

}

static Rigidbody* _Rigidbody_Init(Component* c, 
                                  int collider, 
                                  float mass, 
                                  vec2 forceAccum, 
                                  vec2 velocity, 
                                  bool sensor, 
                                  float cor) {

    Rigidbody* rb = malloc(sizeof(Rigidbody));
    rb->component = c;
    rb->collider = collider;
    rb->mass = mass;
    glm_vec2_copy(forceAccum, rb->forceAccum);
    glm_vec2_copy(velocity, rb->velocity);
    rb->sensor = sensor;
    rb->cor = cor;

    c->serialise = &Rigidbody_Serialise;
    c->free = &Rigidbody_Free;
    c->data = rb;

    return rb;
}

Component* Rigidbody_Init() {
    Component* c = Component_Init("Rigidbody");
    _Rigidbody_Init(c, -1, 1.0f, (vec2) {0,0}, (vec2) {0,0}, 0, 1.0f);
    return c;
}

bool Rigidbody_Load(Component* c, cJSON* json) {

    int collider;
    float mass;
    vec2 forceAccum;
    vec2 velocity;
    bool sensor;
    float cor;

    if (!WIO_ParseInt(json, "collider", &collider)) {return 0;}
    if (!WIO_ParseFloat(json, "mass", &mass)) {return 0;}
    if (!WIO_ParseVec2(json, "forceAccum", forceAccum)) {return 0;}
    if (!WIO_ParseVec2(json, "velocity", velocity)) {return 0;}
    if (!WIO_ParseBool(json, "sensor", &sensor)) {return 0;}
    if (!WIO_ParseFloat(json, "cor", &cor)) {return 0;}

    // Initialise the rigidbody class.
    _Rigidbody_Init(c, collider, mass, forceAccum, velocity, sensor, cor);

    return 1;
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
#include "gravity.h"
#include "rigidbody.h"

ForceGenerator* Gravity_Init(vec2 gravity) {

    ForceGenerator* fg = ForceGenerator_Init("Gravity");
    Gravity* g = malloc(sizeof(Gravity));

    g->fg = fg;
    glm_vec2_copy(gravity, g->gravity);
    
    fg->updateForce = &Gravity_UpdateForce;
    fg->free = &Gravity_Free;
    fg->data = g;
    return fg;
}

void Gravity_UpdateForce(ForceGenerator* fg, Rigidbody* body, float dt) {
    Gravity* g = (Gravity*) fg->data;
    vec2 acceleration;
    glm_vec2_scale(g->gravity, body->mass, acceleration);
    Rigidbody_AddForce(body, acceleration);
}

void Gravity_Free(ForceGenerator* fg) {
    ForceGenerator_Free(fg);
    free(fg->data);
}
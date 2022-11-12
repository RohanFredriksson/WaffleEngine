#include "gravity.h"
#include "rigidbody.h"

ForceGenerator* Gravity_Init(vec2 gravity) {

    ForceGenerator* fg = ForceGenerator_Init("Gravity", &Gravity_UpdateForce, &Gravity_Free);

    // Allocate some memory for gravity.
    Gravity* g = malloc(sizeof(Gravity));

    // Initialise the gravity struct.
    g->fg = fg;
    glm_vec2_copy(gravity, g->gravity);

    // Attach the gravity struct to the forcegenerator
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
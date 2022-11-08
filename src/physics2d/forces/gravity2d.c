#include "gravity2d.h"
#include "rigidbody2d.h"

ForceGenerator* Gravity2D_Init(vec2 gravity) {

    ForceGenerator* fg = ForceGenerator_Init("Gravity2D", &Gravity2D_UpdateForce, &Gravity2D_Free);

    // Allocate some memory for gravity.
    Gravity2D* g = malloc(sizeof(Gravity2D));

    // Initialise the gravity struct.
    g->fg = fg;
    glm_vec2_copy(gravity, g->gravity);

    // Attach the gravity struct to the forcegenerator
    fg->data = g;
    return fg;

}

void Gravity2D_UpdateForce(ForceGenerator* fg, Rigidbody2D* body, float dt) {
    Gravity2D* g = (Gravity2D*) fg->data;
    vec2 acceleration;
    glm_vec2_scale(g->gravity, body->mass, acceleration);
    Rigidbody2D_AddForce(body, acceleration);
}

void Gravity2D_Free(ForceGenerator* fg) {
    ForceGenerator_Free(fg);
    free(fg->data);
}
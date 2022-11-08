#include "external.h"
#include "forcegenerator.h"

#ifndef GRAVITY2D_H
#define GRAVITY2D_H

struct Gravity2D {
    ForceGenerator* fg;
    vec2 gravity;
};
typedef struct Gravity2D Gravity2D;

ForceGenerator* Gravity2D_Init(vec2 gravity);

void Gravity2D_UpdateForce(ForceGenerator* fg, Rigidbody2D* body, float dt);

void Gravity2D_Free(ForceGenerator* fg);

#endif
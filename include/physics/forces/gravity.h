#include "external.h"
#include "forcegenerator.h"

#ifndef GRAVITY_H
#define GRAVITY_H

struct Gravity {
    ForceGenerator* fg;
    vec2 gravity;
};
typedef struct Gravity Gravity;

ForceGenerator* Gravity_Init(vec2 gravity);

void Gravity_UpdateForce(ForceGenerator* fg, Rigidbody* body, float dt);

void Gravity_Free(ForceGenerator* fg);

#endif
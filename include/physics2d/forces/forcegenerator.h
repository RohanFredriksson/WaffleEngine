#include "rigidbody2d.h"

#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H

struct ForceGenerator {
    void (*updateForce)(struct ForceGenerator* fg, Rigidbody2D* body, float dt);
};
typedef struct ForceGenerator ForceGenerator;

#endif
#include "rigidbody.h"

#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H

struct ForceGenerator {
    void (*updateForce)(struct ForceGenerator* fg, Rigidbody* body, float dt);
    void (*free)(struct ForceGenerator* fg);
    char* type;
    void* data;
};
typedef struct ForceGenerator ForceGenerator;

ForceGenerator* ForceGenerator_Init(char* type);

void ForceGenerator_Free(ForceGenerator* fg);

#endif
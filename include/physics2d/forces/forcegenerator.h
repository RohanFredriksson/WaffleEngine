#include "rigidbody2d.h"

#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H

struct ForceGenerator {
    void (*updateForce)(struct ForceGenerator* fg, Rigidbody2D* body, float dt);
    void (*free)(struct ForceGenerator* fg);
    char* type;
    void* data;
};
typedef struct ForceGenerator ForceGenerator;

ForceGenerator* ForceGenerator_Init(char* type, 
                                    void (*updateForce)(ForceGenerator* fg, Rigidbody2D* body, float dt),
                                    void (*free)(ForceGenerator* fg));

void ForceGenerator_Free(ForceGenerator* fg);

#endif
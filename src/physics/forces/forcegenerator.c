#include <stdlib.h>
#include <string.h>
#include "forcegenerator.h"

ForceGenerator* ForceGenerator_Init(char* type, 
                                    void (*updateForce)(ForceGenerator* fg, Rigidbody* body, float dt),
                                    void (*free)(ForceGenerator* fg)) {

    ForceGenerator* fg = (ForceGenerator*) malloc(sizeof(ForceGenerator));

    fg->type = type;
    fg->updateForce = updateForce;
    fg->free = free;

    return fg;
}

void ForceGenerator_Free(ForceGenerator* fg) {
    fg->free(fg);
    free(fg->type);
}

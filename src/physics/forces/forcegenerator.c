#include <stdlib.h>
#include <string.h>
#include "forcegenerator.h"

ForceGenerator* ForceGenerator_Init(char* type, 
                                    void (*updateForce)(ForceGenerator* fg, Rigidbody* body, float dt),
                                    void (*free)(ForceGenerator* fg)) {

    ForceGenerator* fg = (ForceGenerator*) malloc(sizeof(ForceGenerator));

    // Store the type
    fg->type = malloc(strlen(type)+1);
    memcpy(fg->type, type, strlen(type)+1);

    fg->updateForce = updateForce;
    fg->free = free;

    return fg;
}

void ForceGenerator_Free(ForceGenerator* fg) {
    fg->free(fg);
    free(fg->type);
}

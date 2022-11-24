#include <stdlib.h>
#include <string.h>
#include "forcegenerator.h"

ForceGenerator* ForceGenerator_Init(char* type) {

    ForceGenerator* fg = (ForceGenerator*) malloc(sizeof(ForceGenerator));
    fg->updateForce = NULL;
    fg->free = NULL;

    fg->type = type;
    return fg;
}

void ForceGenerator_Free(ForceGenerator* fg) {
    fg->free(fg);
    free(fg->type);
}

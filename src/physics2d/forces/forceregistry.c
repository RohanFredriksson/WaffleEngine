#include <stdlib.h>
#include <string.h>
#include "forceregistration.h"
#include "forceregistry.h"

#define INITIAL_REGISTRY_SIZE 16

void ForceRegistry_Init(ForceRegistry* fr) {
    fr->registry = malloc(INITIAL_REGISTRY_SIZE * sizeof(ForceRegistration));
    fr->sizeRegistry = INITIAL_REGISTRY_SIZE;
    fr->numRegistry = 0;
}

void ForceRegistry_Free(ForceRegistry* fr) {
    free(fr->registry);
}

void ForceRegistry_Add(ForceRegistry* fr, ForceGenerator* fg, Rigidbody2D* rb) {

    ForceRegistration registration;
    ForceRegistration_Init(&registration, fg, rb);

    if (fr->numRegistry >= fr->sizeRegistry) {
        fr->registry = realloc(fr->registry, fr->sizeRegistry * 2 * sizeof(ForceRegistration));
        fr->sizeRegistry = fr->sizeRegistry * 2;
    }

    fr->registry[fr->numRegistry] = registration;
    fr->numRegistry++;

}

void ForceRegistry_Remove(ForceRegistry* fr, ForceGenerator* fg, Rigidbody2D* rb) {

    for (int i = 0; i < fr->numRegistry; i++) {
        ForceRegistration* current = fr->registry + i;
        if (current->fg == fg && current->rb == rb) {
            memmove(current, current+1, fr->numRegistry - i - 1);
            fr->numRegistry--;
            return;
        }
    }

}

void ForceRegistry_Clear(ForceRegistry* fr) {
    ForceRegistry_Free(fr);
    ForceRegistry_Init(fr);
}

void ForceRegistry_UpdateForces(ForceRegistry* fr, float dt) {
    for (int i = 0; i < fr->numRegistry; i++) {
        ForceRegistration* current = fr->registry + i;
        current->fg->updateForce(current->fg, current->rb, dt);
    }
}

void ForceRegistry_ZeroForces(ForceRegistry* fr) {
    for (int i = 0; i < fr->numRegistry; i++) {
        ForceRegistration* current = fr->registry + i;
        //current->rb->zeroForces(current->rb);
    }
}
#include "rigidbody.h"
#include "forceregistration.h"
#include "forcegenerator.h"

#ifndef FORCEREGISTRY_H
#define FORCEREGISTRY_H

struct ForceRegistry {
    ForceRegistration* registry;
    int numRegistry;
    int sizeRegistry;
};
typedef struct ForceRegistry ForceRegistry;

void ForceRegistry_Init(ForceRegistry* fr);
void ForceRegistry_Free(ForceRegistry* fr);
void ForceRegistry_Add(ForceRegistry* fr, ForceGenerator* fg, Rigidbody* rb);
void ForceRegistry_Remove(ForceRegistry* fr, ForceGenerator* fg, Rigidbody* rb);
void ForceRegistry_Clear(ForceRegistry* fr);
void ForceRegistry_UpdateForces(ForceRegistry* fr, float dt);
void ForceRegistry_ZeroForces(ForceRegistry* fr);

#endif
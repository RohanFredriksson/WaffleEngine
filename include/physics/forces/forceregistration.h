#include <stdbool.h>
#include "forcegenerator.h"
#include "rigidbody.h"

#ifndef FORCEREGISTRATION_H
#define FORCEREGISTRATION_H

struct ForceRegistration {
    ForceGenerator* fg;
    Rigidbody* rb;
};
typedef struct ForceRegistration ForceRegistration;

void ForceRegistration_Init(ForceRegistration* fr, ForceGenerator* fg, Rigidbody* rb);

#endif
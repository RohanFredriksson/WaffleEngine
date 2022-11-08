#include <stdbool.h>
#include "forcegenerator.h"
#include "rigidbody2d.h"

#ifndef FORCEREGISTRATION_H
#define FORCEREGISTRATION_H

struct ForceRegistration {
    ForceGenerator* fg;
    Rigidbody2D* rb;
};
typedef struct ForceRegistration ForceRegistration;

void ForceRegistration_Init(ForceRegistration* fr, ForceGenerator* fg, Rigidbody2D* rb);

#endif
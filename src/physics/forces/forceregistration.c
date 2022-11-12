#include "forceregistration.h"

void ForceRegistration_Init(ForceRegistration* fr, ForceGenerator* fg, Rigidbody* rb) {
    fr->fg = fg;
    fr->rb = rb;
}
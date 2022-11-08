#include "forceregistration.h"

void ForceRegistration_Init(ForceRegistration* fr, ForceGenerator* fg, Rigidbody2D* rb) {
    fr->fg = fg;
    fr->rb = rb;
}
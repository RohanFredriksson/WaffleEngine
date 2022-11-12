#include "external.h"
#include "line.h"

void Line_Init(Line* l, vec2 from, vec2 to) {
    glm_vec2_copy(from, l->from);
    glm_vec2_copy(to, l->to);
}

float Line_LengthSquared(Line* l) {
    return glm_vec2_distance2(l->to, l->from);
}
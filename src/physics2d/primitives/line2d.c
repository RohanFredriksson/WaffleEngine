#include "external.h"
#include "line2d.h"

void Line2D_Init(Line2D* l, vec2 from, vec2 to) {
    glm_vec2_copy(from, l->from);
    glm_vec2_copy(to, l->to);
}
#include "external.h"
#include "ray2d.h"

void Ray2D_Init(Ray2D* ray, vec2 origin, vec2 direction) {
    glm_vec2_copy(origin, ray->origin);
    glm_vec2_copy(direction, ray->direction);
    glm_vec2_normalize(ray->direction);
}
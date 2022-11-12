#include "external.h"
#include "ray.h"

void Ray_Init(Ray* ray, vec2 origin, vec2 direction) {
    glm_vec2_copy(origin, ray->origin);
    glm_vec2_copy(direction, ray->direction);
    glm_vec2_normalize(ray->direction);
}
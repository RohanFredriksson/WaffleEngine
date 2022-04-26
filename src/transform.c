#include "external.h"
#include "transform.h"

void Transform_Init(Transform* t, vec2 pos, vec2 size, float rotation) {
    glm_vec2_copy(pos, t->pos);
    glm_vec2_copy(size, t->size);
    t->rotation = rotation;
}

bool Transform_Equals(Transform t1, Transform t2) {

    if (glm_vec2_eqv(t1.pos, t2.pos) && glm_vec2_eqv(t1.size, t2.size) && (t1.rotation == t2.rotation)) {
        return 1;
    }
    return 0;
    
}
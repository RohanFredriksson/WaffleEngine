#include "rigidbody2d.h"

Component* Rigidbody2D_Init(vec2 pos, float rotation) {

    Component* c = Component_Init("Rigidbody2D", &Rigidbody2D_Update, &Rigidbody2D_Free);

    // Allocate some memory for the rigidbody.
    Rigidbody2D* rb = malloc(sizeof(Rigidbody2D));

    // Initialise the rigidbody.
    rb->component = c;
    rb->rawTransform = NULL;
    glm_vec2_copy(pos, rb->pos);
    rb->rotation = rotation;
    rb->mass = 0.0f;
    glm_vec2_zero(rb->forceAccum);
    glm_vec2_zero(rb->linearVelocity);
    rb->angularVelocity = 0.0f;
    rb->linearDamping = 0.0f;
    rb->angularDamping = 0.0f;
    rb->fixedRotation = 0;

    // Attach the rigidbody to the component
    c->data = rb;
    return c;

}

void Rigidbody2D_Update(Component* c, float dt) {
    
}

void Rigidbody2D_Free(Component* c) {
    Component_Free(c);
    free(c->data);
}

void Rigidbody2D_ClearAccumulators(Rigidbody2D* rb) {
    glm_vec2_zero(rb->forceAccum);
}

void Rigidbody2D_SynchCollisionTransforms(Rigidbody2D* rb) {
    if (rb->rawTransform != NULL) {
        glm_vec2_copy(rb->pos, rb->rawTransform->pos);
        rb->rawTransform->rotation = rb->rotation;
    }
}

void Rigidbody2D_PhysicsUpdate(Rigidbody2D* rb, float dt) {
    
    if (rb->mass == 0.0f) {return;}

    // Calculate linear velocity
    vec2 dv;
    glm_vec2_scale(rb->forceAccum, dt/rb->mass, dv);
    glm_vec2_add(rb->linearVelocity, dv, rb->linearVelocity);

    // Calculate linear position
    vec2 ds;
    glm_vec2_scale(rb->linearVelocity, dt, ds);
    glm_vec2_add(rb->pos, ds, rb->pos);

    Rigidbody2D_SynchCollisionTransforms(rb);
    Rigidbody2D_ClearAccumulators(rb);

}

void Rigidbody2D_AddForce(Rigidbody2D* rb, vec2 force) {
    glm_vec2_add(rb->forceAccum, force, rb->forceAccum);
}

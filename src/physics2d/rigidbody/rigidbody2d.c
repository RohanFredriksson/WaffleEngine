#include "rigidbody2d.h"

Component* Rigidbody2D_Init(vec2 pos, float rotation) {

    Component* c = Component_Init("Rigidbody2D", &Rigidbody2D_Update, &Rigidbody2D_Free);

    // Allocate some memory for the rigidbody.
    Rigidbody2D* rb = malloc(sizeof(Rigidbody2D));

    // Initialise the rigidbody.
    rb->component = c;
    glm_vec2_copy(pos, rb->pos);
    rb->rotation = rotation;
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
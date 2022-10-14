#include <stdlib.h>
#include "external.h"
#include "rigidbody.h"
#include "gameobject.h"
#include "transform.h"
#include "bodytype.h"

Component* RigidBody_Init(Transform* transform, BodyType bodyType) {

    Component* c = Component_Init("RigidBody", &RigidBody_Update, &RigidBody_Free);

    // Allocate some memory for the rigid body.
    RigidBody* rb = malloc(sizeof(RigidBody));

    // Initialise the rigid body.
    rb->component = c;

    rb->transform = transform;
    rb->bodyType = bodyType;

    // Attach the rigid body to the component
    c->data = rb;
    return c;

}

void RigidBody_Update(Component* c, float dt) {
    
}

void RigidBody_Free(Component* c) {
    Component_Free(c);
    free(c->data);
}
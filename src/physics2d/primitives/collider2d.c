#include <stdlib.h>
#include "external.h"
#include "collider2d.h"

Component* Collider2D_Init() {
    
    Component* c = Component_Init("Collider2D", &Collider2D_Update, &Collider2D_Free);

    // Allocate some memory for the collider.
    Collider2D* co = malloc(sizeof(Collider2D));

    // Initialise the collider.
    co->component = c;



    // Attach the collider to the component
    c->data = co;
    return c;

}

void Collider2D_Update(Component* c, float dt) {

    Collider2D* co = (Collider2D*) c->data;



}

void Collider2D_Free(Component* c) {
    Component_Free(c);
    free(c->data);
}
#include <stdlib.h>
#include "component.h"
#include "gameobject.h"

int next_id = 0;

void GameObject_Init(GameObject* g, Transform* t) {

    g->id = next_id;
    g->num_components = 0;
    g->size_components = 0;
    g->components = NULL;
    g->transform = t;
    next_id++;

}

void GameObject_Update(GameObject* g, float dt) {
    
    // Update all components.
    for (int i = 0; i < num_components; i++) {
        Component* c = g->components[i];
        if (c->update != NULL) {
            c->update(c, dt);
        }
    }

}

void GameObject_Free(GameObject* g) {
    if (g->num_components != 0) {
        free(g->components);
    }
}

void GameObject_AddComponent(GameObject* g, Component* c) {

    // If there is no space, allocate some.
    if (g->num_components == 0) {
        g->components = (Component**) malloc(16 * sizeof(Component*));
        size_components = 16;
    }

    // If there is no more space, allocate some more.
    else if (g->num_components == g->size_components) {
        g->components = (Component**) realloc(g->components, size_components * 2 * sizeof(Component*));
        size_components = size_components * 2;
    }

    // Add the component.
    g->components[num_components] = c;
    num_components++;

}


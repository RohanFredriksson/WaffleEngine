#include <string.h>
#include <stdlib.h>
#include "gameobject.h"

#define INITIAL_COMPONENTS_SIZE 16
int next_id = 0;

void GameObject_Init(GameObject* g, Transform* t) {

    g->id = next_id;
    g->num_components = 0;
    g->size_components = INITIAL_COMPONENTS_SIZE;
    g->components = malloc(INITIAL_COMPONENTS_SIZE * sizeof(Component));
    g->transform = t;
    next_id++;

}

void GameObject_Update(GameObject* g, float dt) {
    
    // Update all components.
    for (int i = 0; i < g->num_components; i++) {
        Component* c = g->components + i;
        if (c->update != NULL) {
            c->update(c, dt);
        }
    }

}

void GameObject_Free(GameObject* g) {

    // Free all component data in the array.
    for (int i = 0; i < g->num_components; i++) {
        g->components[i].free(g->components + i);
    }

    // Free the array itself.
    free(g->components);

}

void GameObject_AddComponent(GameObject* g, Component* c) {

    // If there is no more space, allocate some more.
    if (g->num_components == g->size_components) {
        g->components = (Component*) realloc(g->components, g->size_components * 2 * sizeof(Component));
        g->size_components = g->size_components * 2;
    }

    // Add the component.
    memcpy(g->components + g->num_components, c, sizeof(Component));
    g->num_components++;

}


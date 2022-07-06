#include <string.h>
#include <stdlib.h>
#include "gameobject.h"

#define INITIAL_COMPONENTS_SIZE 16
int nextGameObjectId = 0;

void GameObject_Init(GameObject* g, Transform* t) {
    g->id = nextGameObjectId;
    g->numComponents = 0;
    g->sizeComponents = INITIAL_COMPONENTS_SIZE;
    g->components = malloc(INITIAL_COMPONENTS_SIZE * sizeof(Component));
    g->transform = t;
    nextGameObjectId++;
}

void GameObject_Update(GameObject* g, float dt) {
    
    // Update all components.
    for (int i = 0; i < g->numComponents; i++) {
        Component* c = g->components + i;
        if (c->update != NULL) {
            c->update(c, dt);
        }
    }

}

void GameObject_Free(GameObject* g) {

    // Free all component data in the array.
    for (int i = 0; i < g->numComponents; i++) {
        g->components[i].free(g->components + i);
    }

    // Free the array itself.
    free(g->components);

}

void GameObject_AddComponent(GameObject* g, Component* c) {

    // If there is no more space, allocate some more.
    if (g->numComponents == g->sizeComponents) {
        g->components = (Component*) realloc(g->components, g->sizeComponents * 2 * sizeof(Component));
        g->sizeComponents = g->sizeComponents * 2;
    }

    // Add the component.
    c->go = g->id;
    memcpy(g->components + g->numComponents, c, sizeof(Component));
    g->numComponents++;

}
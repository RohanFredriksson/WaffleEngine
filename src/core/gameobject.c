#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "gameobject.h"

static int next = 0;

GameObject* GameObject_Init(Transform* t) {
    
    GameObject* g = (GameObject*) malloc(sizeof(GameObject));
    g->id = next;
    List_Init(&g->components, sizeof(Component*));
    g->transform = t;

    next++;
    return g;
}

void GameObject_Update(GameObject* g, float dt) {

    // Update all components.
    Component*** components = (Component***) List_Elements(&g->components);
    int n = List_Length(&g->components);
    for (int i = 0; i < n; i++) {
        Component_Update(*components[i], dt);
    }

}

void GameObject_OnCollision(GameObject* g, GameObject* with, vec2 contact, vec2 normal) {

    // Run on collision of each component
    Component*** components = (Component***) List_Elements(&g->components);
    int n = List_Length(&g->components);
    for (int i = 0; i < n; i++) {
        Component_OnCollision(*components[i], with, contact, normal);
    }

}

void GameObject_Free(GameObject* g) {

    // Free all component data in the array.
    Component*** components = (Component***) List_Elements(&g->components);
    int n = List_Length(&g->components);
    for (int i = 0; i < n; i++) {
        Component_Free(*components[i]);
        free(*components[i]);
    }

    // Free the array itself.
    List_Free(&g->components);

}

void GameObject_AddComponent(GameObject* g, Component* c) {
    c->go = g;
    List_Push(&g->components, &c);
}

void GameObject_RemoveComponent(GameObject* g, Component* c) {

    Component*** components = (Component***) List_Elements(&g->components);
    int n = List_Length(&g->components);
    for (int i = 0; i < n; i++) {
        Component* component = *components[i];
        if (component == c) {
            Component_Free(component);
            free(component);
            List_Remove(&g->components, i);
            break;
        }
    }

}

Component* GameObject_GetComponent(GameObject* g, const char* type) {

    Component*** components = (Component***) List_Elements(&g->components);
    int n = List_Length(&g->components);
    for (int i = 0; i < n; i++) {
        if (strcmp((*components[i])->type, type) == 0) {
            return *components[i];
        }
    }

    return NULL;
}
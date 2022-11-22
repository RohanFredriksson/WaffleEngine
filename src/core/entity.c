#include "entity.h"

static int next = 0;

Entity* Entity_Init(vec2 position, vec2 size, float rotation) {
    
    Entity* e = (Entity*) malloc(sizeof(Entity));
    e->id = next;
    List_Init(&e->components, sizeof(Component*));
    glm_vec2_copy(position, e->position);
    glm_vec2_copy(size, e->size);
    e->rotation = rotation;

    next++;
    return e;
}

void Entity_Update(Entity* e, float dt) {

    // Update all components.
    Component* component;
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &component);
        Component_Update(component, dt);
    }

}

void Entity_OnCollision(Entity* e, Entity* with, vec2 contact, vec2 normal) {

    // Run on collision of each component
    Component* component;
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &component);
        Component_OnCollision(component, with, contact, normal);
    }

}

void Entity_Free(Entity* e) {

    // Free all component data in the array.
    Component* component;
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &component);
        Component_Free(component);
        free(component);
    }

    // Free the array itself.
    List_Free(&e->components);

}

void Entity_AddComponent(Entity* e, Component* c) {
    c->entity = e;
    List_Push(&e->components, &c);
}

void Entity_RemoveComponent(Entity* e, Component* c) {

    Component* component;
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &component);
        if (component == c) {
            Component_Free(component);
            free(component);
            List_Remove(&e->components, i);
            break;
        }
    }

}

Component* Entity_GetComponent(Entity* e, char* type) {

    Component* component;
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &component);
        if (strcmp(component->type, type) == 0) {
            return component;
        }
    }

    return NULL;
}
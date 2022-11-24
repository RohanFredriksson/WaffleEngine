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

Component* Entity_GetComponentByID(Entity* e, int id) {

    Component* component;
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &component);
        if (component->id == id) {
            return component;
        }
    }

    return NULL;

}

Entity* Entity_Parse(cJSON* json) {

    int id;
    vec2 position;
    vec2 size;
    float rotation;

    if (!WIO_ParseInt(json, "id", &id)) {return NULL;}
    if (!WIO_ParseVec2(json, "position", position)) {return NULL;}
    if (!WIO_ParseVec2(json, "size", size)) {return NULL;}
    if (!WIO_ParseFloat(json, "rotation", &rotation)) {return NULL;}

    Entity* e = malloc(sizeof(Entity));
    e->id = id;
    glm_vec2_copy(position, e->position);
    glm_vec2_copy(size, e->size);
    e->rotation = rotation;

    List_Init(&e->components, sizeof(Component*));
    cJSON* components = cJSON_GetObjectItemCaseSensitive(json, "components");
    if (components != NULL && cJSON_IsArray(components)) {
        cJSON* component = NULL;
        cJSON_ArrayForEach(component, components) {
            Component* c = Component_Load(component);
            if (c != NULL) {Entity_AddComponent(e, c);}
        }
    }

    WMath_MaxFloat(id, next);
    return e;

}

cJSON* Entity_Serialise(Entity* e) {

    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "id", e->id);
    cJSON* components = cJSON_CreateArray();
    Component* c; 
    int n = List_Length(&e->components);
    for (int i = 0; i < n; i++) {
        List_Get(&e->components, i, &c);
        cJSON* component = Component_Serialise(c);
        cJSON_AddItemToArray(components, component);
    }
    cJSON_AddItemToObject(json, "components", components);
    WIO_AddVec2(json, "position", e->position);
    WIO_AddVec2(json, "size", e->size);
    WIO_AddFloat(json, "rotation", e->rotation);
    return json;

}
#include "entity.h"

static int next = 0;

static Entity* _Entity_Init(int id, bool dead, vec2 position, vec2 size, float rotation) {

    Entity* e = (Entity*) malloc(sizeof(Entity));
    e->id = id;
    e->dead = dead;
    e->ignore = 0;
    List_Init(&e->components, sizeof(Component*));
    List_Init(&e->newComponents, sizeof(Component*));
    glm_vec2_copy(position, e->position);
    glm_vec2_copy(size, e->size);
    e->rotation = rotation;

    next = WMath_MaxFloat(next+1, id+1);
    return e;

}

Entity* Entity_Init(vec2 position, vec2 size, float rotation) {
    return _Entity_Init(next, 0, position, size, rotation);
}

void Entity_Update(Entity* e, float dt) {

    // Update all components.
    Component* component;
    for (int i = 0; i < List_Length(&e->components); i++) {

        List_Get(&e->components, i, &component);
        Component_Update(component, dt);

        // If the component is dead, remove it.
        if (component->dead) {
            Component_Free(component);
            free(component);
            List_Remove(&e->components, i);
            i--;
        }

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
    List_Free(&e->newComponents);

}

void Entity_Kill(Entity* e) {
    e->dead = 1; // Flags for the scene to remove this entity at the end of its update call.
}

void Entity_Ignore(Entity* e) {
    e->ignore = 1;
}

void Entity_Unignore(Entity* e) {
    e->ignore = 0;
}

void Entity_AddComponent(Entity* e, Component* c) {
    c->entity = e;
    List_Push(&e->components, &c);
    List_Push(&e->newComponents, &c);
}

bool Entity_HasNewComponents(Entity* e) {
    return List_Length(&e->newComponents) != 0;
}

void Entity_ClearNewComponents(Entity* e) {
    List_Clear(&e->newComponents);
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
    bool dead;
    vec2 position;
    vec2 size;
    float rotation;

    if (!WIO_ParseInt(json, "id", &id)) {return NULL;}
    if (!WIO_ParseBool(json, "dead", &dead)) {return NULL;}
    if (!WIO_ParseVec2(json, "position", position)) {return NULL;}
    if (!WIO_ParseVec2(json, "size", size)) {return NULL;}
    if (!WIO_ParseFloat(json, "rotation", &rotation)) {return NULL;}

    Entity* e = _Entity_Init(id, dead, position, size, rotation);
    cJSON* components = cJSON_GetObjectItemCaseSensitive(json, "components");
    if (components != NULL && cJSON_IsArray(components)) {
        cJSON* component = NULL;
        cJSON_ArrayForEach(component, components) {
            Component* c = Component_Load(component);
            if (c != NULL) {Entity_AddComponent(e, c);}
        }
    }

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
        if (!c->ignore) {
            cJSON* component = Component_Serialise(c);
            cJSON_AddItemToArray(components, component);
        }
    }
    cJSON_AddItemToObject(json, "components", components);
    WIO_AddBool(json, "dead", e->dead);
    WIO_AddVec2(json, "position", e->position);
    WIO_AddVec2(json, "size", e->size);
    WIO_AddFloat(json, "rotation", e->rotation);
    return json;

}
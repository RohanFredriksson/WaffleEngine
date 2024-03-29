#include <stdlib.h>
#include <string.h>
#include "wmath.h"
#include "box.h"

static cJSON* Box_Serialise(Collider* co) {

    Box* b = (Box*) co->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddVec2(json, "size", b->size);
    return json;

}

static Box* _Box_Init(Collider* collider, vec2 size) {
    
    Box* box = malloc(sizeof(Box));

    box->collider = collider;
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(size, 0.5f, box->halfSize);

    collider->serialise = &Box_Serialise;
    collider->data = box;

    return box;
}

Component* Box_Init(vec2 size) {

    Component* component = Collider_Init("Box");
    Collider* collider = (Collider*) component->data;
    _Box_Init(collider, size);
    
    return component;
}

bool Box_Load(Collider* co, cJSON* json) {

    vec2 size;
    if (!WIO_ParseVec2(json, "size", size)) {return 0;}

    // Initialise the box class.
    _Box_Init(co, size);

    return 1;
}

Component* Box_GetRigidbody(Box* box) {
    return Collider_GetRigidbody(box->collider);
}

void Box_GetMin(Box* box, vec2 dest) {
    vec2 position;
    Component_GetPosition(Box_GetRigidbody(box), position);
    glm_vec2_sub(position, box->halfSize, dest);
}

void Box_GetMax(Box* box, vec2 dest) {
    vec2 position;
    Component_GetPosition(Box_GetRigidbody(box), position);
    glm_vec2_add(position, box->halfSize, dest);
}

void Box_GetVertices(Box* box, vec2* buffer) {

    vec2 min;
    vec2 max;
    Box_GetMin(box, min);
    Box_GetMax(box, max);

    vec2 vertices[4];
    vertices[0][0] = min[0];
    vertices[0][1] = min[1];
    vertices[1][0] = min[0];
    vertices[1][1] = max[1];
    vertices[2][0] = max[0];
    vertices[2][1] = min[1];
    vertices[3][0] = max[0];
    vertices[3][1] = max[1];
    memcpy(vertices, buffer, sizeof(vec2) * 4);

}

void Box_SetSize(Box* box, vec2 size) {
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(box->size, 0.5f, box->halfSize);
}
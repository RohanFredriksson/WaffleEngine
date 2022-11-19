#include <stdlib.h>
#include <string.h>
#include "wmath.h"
#include "box.h"

Component* Box_Init(vec2 size, Component* rigidbody) {

    if (rigidbody != NULL && strcmp(rigidbody->type, "Rigidbody") != 0) {
        printf("ERROR::BOX::INIT::SUPPLIED_COMPONENT_NOT_RIGIDBODY\n");
    }

    Component* component = Collider_Init("Box", NULL, NULL, NULL);
    Collider* collider = (Collider*) component->data;
    Box* box = malloc(sizeof(Box));

    box->collider = collider;
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(size, 0.5f, box->halfSize);
    box->rigidbody = rigidbody->data;

    collider->data = box;
    return component;

}

void Box_GetMin(Box* box, vec2 dest) {
    glm_vec2_sub(box->rigidbody->transform->pos, box->halfSize, dest);
}

void Box_GetMax(Box* box, vec2 dest) {
    glm_vec2_add(box->rigidbody->transform->pos, box->halfSize, dest);
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

void Box_SetRigidbody(Box* box, Rigidbody* rb) {
    box->rigidbody = rb;
}
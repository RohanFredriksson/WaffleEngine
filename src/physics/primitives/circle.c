#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "entity.h"
#include "circle.h"

Component* Circle_Init(float radius, Component* rigidbody) {

    if (rigidbody != NULL && strcmp(rigidbody->type, "Rigidbody") != 0) {
        printf("ERROR::CIRCLE::INIT::SUPPLIED_COMPONENT_NOT_RIGIDBODY\n");
    }

    Component* component = Collider_Init("Circle", (Rigidbody*) rigidbody->data);
    Collider* collider = (Collider*) component->data;
    Circle* circle = malloc(sizeof(Circle));

    circle->collider = collider;
    circle->radius = radius;

    collider->serialise = &Circle_Serialise;
    collider->data = circle;
    return component;

}

cJSON* Circle_Serialise(Collider* co) {

    Circle* c = (Circle*) co->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddFloat(json, "radius", c->radius);
    return json;

}

Component* Circle_GetRigidbody(Circle* c) {
    return Collider_GetRigidbody(c->collider);
}

void Circle_SetRadius(Circle* c, float radius) {
    c->radius = radius;
}

void Circle_SetRigidbody(Circle* c, Rigidbody* rb) {
    Collider_SetRigidbody(c->collider, rb);
}
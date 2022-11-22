#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "entity.h"
#include "circle.h"

Component* Circle_Init(float radius, Component* rigidbody) {

    if (rigidbody != NULL && strcmp(rigidbody->type, "Rigidbody") != 0) {
        printf("ERROR::CIRCLE::INIT::SUPPLIED_COMPONENT_NOT_RIGIDBODY\n");
    }

    Component* component = Collider_Init("Circle", NULL, NULL, &Circle_Serialise, NULL);
    Collider* collider = (Collider*) component->data;
    Circle* circle = malloc(sizeof(Circle));

    circle->collider = collider;
    circle->radius = radius;
    circle->rigidbody = rigidbody->id;

    collider->data = circle;
    return component;

}

cJSON* Circle_Serialise(Collider* co) {

    Circle* c = (Circle*) co->data;

    cJSON* json = cJSON_CreateObject();

    cJSON* rigidbody = cJSON_CreateNumber((double) c->rigidbody);
    cJSON_AddItemToObject(json, "rigidbody", rigidbody);

    cJSON* radius = cJSON_CreateNumber((double) c->radius);
    cJSON_AddItemToObject(json, "radius", radius);

    return json;

}

Component* Circle_GetRigidbody(Circle* c) {
    return Entity_GetComponentByID(c->collider->component->entity, c->rigidbody);
}

void Circle_SetRadius(Circle* c, float radius) {
    c->radius = radius;
}

void Circle_SetRigidbody(Circle* c, Rigidbody* rb) {
    c->rigidbody = rb->component->id;
}
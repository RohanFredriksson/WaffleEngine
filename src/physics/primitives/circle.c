#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "entity.h"
#include "circle.h"

static cJSON* Circle_Serialise(Collider* co) {

    Circle* c = (Circle*) co->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddFloat(json, "radius", c->radius);
    return json;

}

static Circle* _Circle_Init(Collider* collider, float radius) {

    Circle* circle = malloc(sizeof(Circle));

    circle->collider = collider;
    circle->radius = radius;

    collider->serialise = &Circle_Serialise;
    collider->data = circle;

    return circle;
}

Component* Circle_Init(float radius) {

    Component* component = Collider_Init("Circle");

    Collider* collider = (Collider*) component->data;
    _Circle_Init(collider, radius);

    return component;
}

bool Circle_Load(Collider* co, cJSON* json) {

    float radius;
    if (!WIO_ParseFloat(json, "radius", &radius)) {return 0;}

    // Initialise the circle class.
    _Circle_Init(co, radius);

    return 1;
}

Component* Circle_GetRigidbody(Circle* c) {
    return Collider_GetRigidbody(c->collider);
}

void Circle_SetRadius(Circle* c, float radius) {
    c->radius = radius;
}
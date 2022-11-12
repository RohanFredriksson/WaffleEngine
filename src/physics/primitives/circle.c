#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gameobject.h"
#include "circle.h"

Component* Circle_Init(float radius, Component* rigidbody) {

    if (rigidbody != NULL && strcmp(rigidbody->type, "Rigidbody") != 0) {
        printf("ERROR::CIRCLE::INIT::SUPPLIED_COMPONENT_NOT_RIGIDBODY\n");
    }

    Component* component = Collider_Init("Circle", &Circle_Update, &Circle_Free);
    Collider* collider = (Collider*) component->data;
    Circle* circle = malloc(sizeof(Circle));

    circle->collider = collider;
    circle->radius = radius;
    circle->rigidbody = rigidbody->data;

    collider->data = circle;
    return component;

}

void Circle_Update(Collider* c, float dt) {

}

void Circle_Free(Collider* c) {
    
}

void Circle_SetRadius(Circle* c, float radius) {
    c->radius = radius;
}

void Circle_SetRigidbody(Circle* c, Rigidbody* rb) {
    c->rigidbody = rb;
}
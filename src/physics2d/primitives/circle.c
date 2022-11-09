#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gameobject.h"
#include "circle.h"

Component* Circle_Init(float radius, Component* rigidbody) {

    if (rigidbody != NULL && strcmp(rigidbody->type, "Rigidbody2D") != 0) {
        printf("ERROR::CIRCLE::INIT::SUPPLIED_COMPONENT_NOT_RIGIDBODY\n");
    }

    Component* component = Collider2D_Init("Circle", &Circle_Update, &Circle_Free);
    Collider2D* collider = (Collider2D*) component->data;
    Circle* circle = malloc(sizeof(Circle));

    circle->collider = collider;
    circle->radius = radius;
    circle->rigidbody = rigidbody->data;

    collider->data = circle;
    return component;

}

void Circle_Update(Collider2D* c, float dt) {

}

void Circle_Free(Collider2D* c) {
    
}

void Circle_SetRadius(Circle* c, float radius) {
    c->radius = radius;
}

void Circle_SetRigidbody(Circle* c, Rigidbody2D* rb) {
    c->rigidbody = rb;
}
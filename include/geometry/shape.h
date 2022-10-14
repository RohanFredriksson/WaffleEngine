#include "external.h"
#include "transform.h"

#ifndef SHAPE_H
#define SHAPE_H

struct Rectangle {
    vec2 pos;
    vec2 size;
    float rotation;
};
typedef struct Rectangle Rectangle;

struct Circle {
    vec2 pos;
    float radius;
};
typedef struct Circle Circle;

enum ShapeType {
    RECTANGLE = 0;
    CIRCLE = 1;
}

union Shapes {
    struct Rectangle rectangle;
    struct Circle circle;
};

struct Shape {
    enum ShapeType type;
    union Shapes shape;
}

#endif
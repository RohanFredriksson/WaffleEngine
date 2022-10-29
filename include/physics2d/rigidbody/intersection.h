#include <stdbool.h>
#include "external.h"
#include "line2d.h"
#include "circle.h"
#include "aabb2d.h"
#include "box2d.h"

#ifndef INTERSECTION_H
#define INTERSECTION_H

bool Intersection_PointOnLine(vec2 point, Line2D line);
bool Intersection_PointInCircle(vec2 point, Circle circle);
bool Intersection_PointInAABB2D(vec2 point, AABB2D box);
bool Intersection_PointInBox2D(vec2 point, Box2D box);

#endif
#include <stdbool.h>
#include "external.h"
#include "line2d.h"
#include "circle.h"
#include "aabb2d.h"
#include "box2d.h"
#include "ray2d.h"
#include "raycastresult.h"

#ifndef INTERSECTION_H
#define INTERSECTION_H

bool Intersection_PointOnLine(vec2 point, Line2D line);
bool Intersection_PointInCircle(vec2 point, Circle circle);
bool Intersection_PointInAABB2D(vec2 point, AABB2D box);
bool Intersection_PointInBox2D(vec2 point, Box2D box);
bool Intersection_LineAndCircle(Line2D line, Circle circle);
bool Intersection_LineAndAABB2D(Line2D line, AABB2D box);
bool Intersection_LineAndBox2D(Line2D line, Box2D box);

bool Intersection_RaycastCircle(Circle circle, Ray2D ray, RaycastResult* result);
bool Intersection_RaycastAABB2D(AABB2D box, Ray2D ray, RaycastResult* result);
bool Intersection_RaycastBox2D(Box2D box, Ray2D ray, RaycastResult* result);

#endif
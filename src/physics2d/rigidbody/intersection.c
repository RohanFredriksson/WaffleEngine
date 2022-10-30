#include "intersection.h"
#include "wmath.h"

bool Intersection_PointOnLine(vec2 point, Line2D line) {
    float dy = line.to[1] - line.from[1];
    float dx = line.to[0] - line.from[0];
    float m = dy/dx;
    float b = line.to[1] - (m * line.to[0]) ;
    return point[1] == m * point[0] + b;
}

bool Intersection_PointInCircle(vec2 point, Circle circle) {
    float distanceSquared = glm_vec2_distance2(point, circle.rigidbody->pos);
    return distanceSquared <= circle.radius * circle.radius;
}

bool Intersection_PointInAABB2D(vec2 point, AABB2D box) {

    vec2 min;
    vec2 max;
    AABB2D_GetMin(&box, min);
    AABB2D_GetMax(&box, max);
    return point[0] >= min[0] && point[0] <= max[0] && point[1] >= min[1] && point[1] <= max[1];

}

bool Intersection_PointInBox2D(vec2 point, Box2D box) {
    
    vec2 pointLocalBoxSpace;
    glm_vec2_copy(point, pointLocalBoxSpace);
    WMath_Rotate(pointLocalBoxSpace, box.rigidbody->rotation, box.rigidbody->pos);

    vec2 min;
    vec2 max;
    Box2D_GetMin(&box, min);
    Box2D_GetMax(&box, max);
    return pointLocalBoxSpace[0] >= min[0] && pointLocalBoxSpace[0] <= max[0] && pointLocalBoxSpace[1] >= min[1] && pointLocalBoxSpace[1] <= max[1];

}
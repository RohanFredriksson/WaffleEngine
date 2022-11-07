#include <float.h>
#include <math.h>
#include "intersection.h"
#include "wmath.h"

void Intersection_GetIntervalAABB2D(AABB2D rect, vec2 axis, vec2 dest) {
    
    glm_vec2_zero(dest);
    
    vec2 min;
    vec2 max;
    AABB2D_GetMin(&rect, min);
    AABB2D_GetMax(&rect, max);

    vec2 vertices[4];
    vertices[0][0] = min[0];
    vertices[0][1] = min[1];
    vertices[1][0] = min[0];
    vertices[1][1] = max[1];
    vertices[2][0] = max[0];
    vertices[2][1] = min[1];
    vertices[3][0] = max[0];
    vertices[3][1] = max[1];

    dest[0] = glm_vec2_dot(axis, vertices[0]);
    dest[1] = dest[0];
    for (int i = 0; i < 4; i++) {
        float projection = glm_vec2_dot(axis, vertices[i]);
        if (projection < dest[0]) {dest[0] = projection;}
        if (projection > dest[1]) {dest[1] = projection;}
    }

}

void Intersection_GetIntervalBox2D(Box2D rect, vec2 axis, vec2 dest) {
    
    glm_vec2_zero(dest);

    vec2 vertices[4];
    Box2D_GetVertices(&rect, vertices);

    dest[0] = glm_vec2_dot(axis, vertices[0]);
    dest[1] = dest[0];
    for (int i = 0; i < 4; i++) {
        float projection = glm_vec2_dot(axis, vertices[i]);
        if (projection < dest[0]) {dest[0] = projection;}
        if (projection > dest[1]) {dest[1] = projection;}
    }

}

bool Intersection_OverlapOnAxisAABB2DAndAABB2D(AABB2D b1, AABB2D b2, vec2 axis) {
    
    vec2 i1;
    vec2 i2;
    Intersection_GetIntervalAABB2D(b1, axis, i1);
    Intersection_GetIntervalAABB2D(b2, axis, i2);

    return (i2[0] <= i1[1]) && (i1[0] <= i2[1]);

}

bool Intersection_OverlapOnAxisAABB2DAndBox2D(AABB2D b1, Box2D b2, vec2 axis) {
    
    vec2 i1;
    vec2 i2;
    Intersection_GetIntervalAABB2D(b1, axis, i1);
    Intersection_GetIntervalBox2D(b2, axis, i2);

    return (i2[0] <= i1[1]) && (i1[0] <= i2[1]);

}

bool Intersection_OverlapOnAxisBox2DAndBox2D(Box2D b1, Box2D b2, vec2 axis) {
    
    vec2 i1;
    vec2 i2;
    Intersection_GetIntervalBox2D(b1, axis, i1);
    Intersection_GetIntervalBox2D(b2, axis, i2);

    return (i2[0] <= i1[1]) && (i1[0] <= i2[1]);

}

bool Intersection_PointOnLine(vec2 point, Line2D line) {
    
    float dy = line.to[1] - line.from[1];
    float dx = line.to[0] - line.from[0];
    if (dx == 0.0f) {return point[0] == line.from[0];}

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

bool Intersection_LineAndCircle(Line2D line, Circle circle) {

    if (Intersection_PointInCircle(line.from, circle) || Intersection_PointInCircle(line.to, circle)) {
        return 1;
    }

    vec2 ab;
    glm_vec2_sub(line.to, line.from, ab);

    // Project the point (circle position) onto the line ab (line segment).
    // Parameterised position t
    vec2 circleCentre;
    glm_vec2_copy(circle.rigidbody->pos, circleCentre);
    vec2 circleToLineStart;
    glm_vec2_sub(circleCentre, line.from, circleToLineStart);
    float t = glm_vec2_dot(circleToLineStart, ab) /  glm_vec2_dot(ab, ab);

    // Not on line segment
    if (t < 0.0f || t > 1.0f) {
        return 0;
    }

    // Find the closest point to line segment
    vec2 closestPoint;
    vec2 abt;
    glm_vec2_scale(ab, t, abt);
    glm_vec2_add(line.from, abt, closestPoint);

    return Intersection_PointInCircle(closestPoint, circle);

}

bool Intersection_CircleAndLine(Circle circle, Line2D line) {
    return Intersection_LineAndCircle(line, circle);
}

bool Intersection_LineAndAABB2D(Line2D line, AABB2D box) {

    if (Intersection_PointInAABB2D(line.from, box) || Intersection_PointInAABB2D(line.to, box)) {
        return 1;
    }

    vec2 unitVector;
    glm_vec2_sub(line.to, line.from, unitVector);
    glm_vec2_normalize(unitVector);
    unitVector[0] = (unitVector != 0) ? 1.0f / unitVector[0] : 0.0f;
    unitVector[1] = (unitVector != 0) ? 1.0f / unitVector[1] : 0.0f;

    vec2 min;
    AABB2D_GetMin(&box, min);
    glm_vec2_sub(min, line.from, min);
    glm_vec2_mul(min, unitVector, min);

    vec2 max;
    AABB2D_GetMax(&box, max);
    glm_vec2_sub(max, line.from, max);
    glm_vec2_mul(max, unitVector, max);

    float tmin = WMath_MaxFloat(WMath_MinFloat(min[0], max[0]), WMath_MinFloat(min[1], max[1]));
    float tmax = WMath_MinFloat(WMath_MaxFloat(min[0], max[0]), WMath_MaxFloat(min[1], max[1]));
    if (tmax < 0 || tmin > tmax) {
        return 0;
    }

    float t = (tmin < 0.0f) ? tmax : tmin;
    return t > 0.0f && t * t < Line2D_LengthSquared(&line);

}

bool Intersection_LineAndBox2D(Line2D line, Box2D box) {

    float theta = -box.rigidbody->rotation;
    vec2 centre; 
    glm_vec2_copy(box.rigidbody->pos, centre);
    vec2 localStart;
    glm_vec2_copy(line.from, localStart);
    vec2 localEnd;
    glm_vec2_copy(line.to, localEnd);

    WMath_Rotate(localStart, theta, centre);
    WMath_Rotate(localEnd, theta, centre);

    Line2D localLine;
    Line2D_Init(&localLine, localStart, localEnd);
    AABB2D aabb;
    vec2 min;
    vec2 max;
    Box2D_GetMin(&box, min);
    Box2D_GetMax(&box, max);
    AABB2D_InitRange(&aabb, min, max);

    return Intersection_LineAndAABB2D(localLine, aabb);

}

bool Intersection_CircleAndCircle(Circle c1, Circle c2) {
    vec2 vecBetweenCentres;
    glm_vec2_sub(c1.rigidbody->pos, c2.rigidbody->pos, vecBetweenCentres);
    float radiiSum = c1.radius + c2.radius;
    return glm_vec2_norm2(vecBetweenCentres) < radiiSum;
}

bool Intersection_CircleAndAABB2D(Circle circle, AABB2D box) {

    vec2 min;
    vec2 max;
    AABB2D_GetMin(&box, min);
    AABB2D_GetMax(&box, max);

    vec2 closestPointToCircle;
    glm_vec2_copy(circle.rigidbody->pos, closestPointToCircle);

    if (closestPointToCircle[0] < min[0]) {closestPointToCircle[0] = min[0];} 
    else if (closestPointToCircle[0] > max[0]) {closestPointToCircle[0] = max[0];}

    if (closestPointToCircle[1] < min[1]) {closestPointToCircle[1] = min[1];} 
    else if (closestPointToCircle[1] > max[1]) {closestPointToCircle[1] = max[1];}

    vec2 circleToBox;
    glm_vec2_sub(circle.rigidbody->pos, closestPointToCircle, circleToBox);
    return glm_vec2_norm2(circleToBox) < circle.radius * circle.radius;

}

bool Intersection_AABB2DAndCircle(AABB2D box, Circle circle) {
    return Intersection_CircleAndAABB2D(circle, box);
}

bool Intersection_CircleAndBox2D(Circle circle, Box2D box) {

    // Treat the box just like an AABB2D, after we rotate the stuff.
    vec2 min;
    min[0] = 0.0f;
    min[1] = 0.0f;

    vec2 max;
    glm_vec2_copy(box.halfSize, max);
    glm_vec2_scale(max, 2.0f, max);

    // Create a circle in box's local space
    vec2 r;
    glm_vec2_sub(circle.rigidbody->pos, box.rigidbody->pos, r);
    WMath_Rotate(r, -box.rigidbody->rotation, min);
    vec2 localCirclePos;
    glm_vec2_add(r, box.halfSize, localCirclePos);

    vec2 closestPointToCircle;
    glm_vec2_copy(localCirclePos, closestPointToCircle);

    if (closestPointToCircle[0] < min[0]) {closestPointToCircle[0] = min[0];} 
    else if (closestPointToCircle[0] > max[0]) {closestPointToCircle[0] = max[0];}

    if (closestPointToCircle[1] < min[1]) {closestPointToCircle[1] = min[1];} 
    else if (closestPointToCircle[1] > max[1]) {closestPointToCircle[1] = max[1];}

    vec2 circleToBox;
    glm_vec2_sub(localCirclePos, closestPointToCircle, circleToBox);
    return glm_vec2_norm2(circleToBox) < circle.radius * circle.radius;

}

bool Intersection_AABB2DAndAABB2D(AABB2D b1, AABB2D b2) {

    vec2 axes[2];
    axes[0][0] = 0;
    axes[0][1] = 1;
    axes[1][0] = 1;
    axes[1][1] = 0;

    for (int i = 0; i < 2; i++) {
        if (!Intersection_OverlapOnAxisAABB2DAndAABB2D(b1, b2, axes[i])) {
            return 0;
        }
    }

    return 1;
}

bool Intersection_AABB2DAndBox2D(AABB2D b1, Box2D b2) {

    vec2 axes[4];
    axes[0][0] = 0;
    axes[0][1] = 1;
    axes[1][0] = 1;
    axes[1][1] = 0;
    axes[2][0] = 0;
    axes[2][1] = 1;
    axes[3][0] = 1;
    axes[3][1] = 0;

    WMath_Rotate(axes[2], b2.rigidbody->rotation, (vec2) { 0.0f, 0.0f });
    WMath_Rotate(axes[3], b2.rigidbody->rotation, (vec2) { 0.0f, 0.0f });

    for (int i = 0; i < 4; i++) {
        if (!Intersection_OverlapOnAxisAABB2DAndBox2D(b1, b2, axes[i])) {
            return 0;
        }
    }

    return 1;

}

bool Intersection_RaycastCircle(Circle circle, Ray2D ray, RaycastResult* result) {
    
    RaycastResult_Reset(result);
    
    vec2 originToCircle;
    glm_vec2_sub(circle.rigidbody->pos, ray.origin, originToCircle);
    float radiusSquared = circle.radius * circle.radius;
    float originToCircleLengthSquared = glm_vec2_norm2(originToCircle);

    // Project the vector from the ray origin onto the direction of the ray
    float a = glm_vec2_dot(originToCircle, ray.direction);
    float b2 = originToCircleLengthSquared - (a * a);
    if (radiusSquared - b2 < 0.0f) {
        return false;
    }

    float f = sqrtf(radiusSquared - b2);
    float t = 0;
    if (originToCircleLengthSquared < radiusSquared) {t = a + f;} // Ray starts inside the circle
    else {t = a - f;}

    if (result != NULL) {
        vec2 point;
        glm_vec2_scale(ray.direction, t, point);
        glm_vec2_add(ray.origin, point, point);
        vec2 normal;
        glm_vec2_sub(point, circle.rigidbody->pos, normal);
        glm_vec2_normalize(normal);
        RaycastResult_Init(result, point, normal, t, 1);
    }
    
    return 1;
}

bool Intersection_RaycastAABB2D(AABB2D box, Ray2D ray, RaycastResult* result) {

    RaycastResult_Reset(result);

    vec2 unitVector;
    glm_vec2_copy(ray.direction, unitVector);
    glm_vec2_normalize(unitVector);
    unitVector[0] = (unitVector != 0) ? 1.0f / unitVector[0] : 0.0f;
    unitVector[1] = (unitVector != 0) ? 1.0f / unitVector[1] : 0.0f;

    vec2 min;
    AABB2D_GetMin(&box, min);
    glm_vec2_sub(min, ray.origin, min);
    glm_vec2_mul(min, unitVector, min);

    vec2 max;
    AABB2D_GetMax(&box, max);
    glm_vec2_sub(max, ray.origin, max);
    glm_vec2_mul(max, unitVector, max);

    float tmin = WMath_MaxFloat(WMath_MinFloat(min[0], max[0]), WMath_MinFloat(min[1], max[1]));
    float tmax = WMath_MinFloat(WMath_MaxFloat(min[0], max[0]), WMath_MaxFloat(min[1], max[1]));
    if (tmax < 0 || tmin > tmax) {
        return 0;
    }

    float t = (tmin < 0.0f) ? tmax : tmin;
    bool hit = t > 0.0f;// && t * t < ray.maximum;
    if (!hit) {
        return 0;
    }

    if (result != NULL) {
        vec2 point;
        glm_vec2_scale(ray.direction, t, point);
        glm_vec2_add(ray.origin, point, point);
        vec2 normal;
        glm_vec2_sub(ray.origin, point, normal);
        glm_vec2_normalize(normal);
        RaycastResult_Init(result, point, normal, t, 1);
    }

    return 1;
}

bool Intersection_RaycastBox2D(Box2D box, Ray2D ray, RaycastResult* result) {

    RaycastResult_Reset(result);

    vec2 size;
    glm_vec2_copy(box.halfSize, size);
    vec2 xAxis;
    xAxis[0] = 1;
    xAxis[1] = 0;
    vec2 yAxis;
    yAxis[0] = 0;
    yAxis[1] = 1;
    vec2 origin;
    glm_vec2_zero(origin);

    WMath_Rotate(xAxis, -box.rigidbody->rotation, origin);
    WMath_Rotate(yAxis, -box.rigidbody->rotation, origin);

    vec2 p;
    glm_vec2_sub(box.rigidbody->pos, ray.origin, p);

    // Project the direction of the ray onto each axis of the box
    vec2 f;
    f[0] = glm_vec2_dot(xAxis, ray.direction);
    f[1] = glm_vec2_dot(yAxis, ray.direction);

    // Project p onto every axis of the box
    vec2 e;
    e[0] = glm_vec2_dot(xAxis, p);
    e[1] = glm_vec2_dot(yAxis, p);

    float tArr[] = {0,0,0,0};
    for (int i = 0; i < 2; i++) {

        if (WMath_CompareFloat(f[i], 0)) {
            if (-e[i] - size[i] > 0 || -e[i] + size[i] < 0) {return false;} // If the ray is parallel to the current axis, and the origin of the ray is not inside, we have no hit.
            f[i] = 0.000001f; // Set to small value to avoid divide by zero.
        }

        tArr[i * 2 + 0] = (e[i] + size[i]) / f[i]; // tmin for this axis
        tArr[i * 2 + 1] = (e[i] - size[i]) / f[i]; // tmax for this axis

    }

    float tmin = WMath_MaxFloat(WMath_MinFloat(tArr[0], tArr[1]), WMath_MinFloat(tArr[2], tArr[3]));
    float tmax = WMath_MinFloat(WMath_MaxFloat(tArr[0], tArr[1]), WMath_MaxFloat(tArr[2], tArr[3]));
    if (tmax < 0 || tmin > tmax) {
        return 0;
    }

    float t = (tmin < 0.0f) ? tmax : tmin;
    bool hit = t > 0.0f;// && t * t < ray.maximum;
    if (!hit) {
        return 0;
    }

    if (result != NULL) {
        vec2 point;
        glm_vec2_scale(ray.direction, t, point);
        glm_vec2_add(ray.origin, point, point);
        vec2 normal;
        glm_vec2_sub(ray.origin, point, normal);
        glm_vec2_normalize(normal);
        RaycastResult_Init(result, point, normal, t, 1);
    }

    return 1;
}
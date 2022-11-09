#include "circle.h"
#include "collider2d.h"
#include "collisionmanifold.h"

#ifndef COLLISION_H
#define COLLISION_H

CollisionManifold* Collision_FindCollisionFeatures(Collider2D* c1, Collider2D* c2);
CollisionManifold* Collision_FindCollisionFeaturesCircleAndCircle(Circle* a, Circle* b);

#endif
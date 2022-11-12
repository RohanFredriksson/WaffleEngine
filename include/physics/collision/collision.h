#include "circle.h"
#include "collider.h"
#include "collisionmanifold.h"

#ifndef COLLISION_H
#define COLLISION_H

CollisionManifold* Collision_FindCollisionFeatures(Collider* c1, Collider* c2);
CollisionManifold* Collision_FindCollisionFeaturesCircleAndCircle(Circle* a, Circle* b);

#endif
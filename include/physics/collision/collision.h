#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "primitives.h"
#include "rigidbody.h"
#include "collisionmanifold.h"
#include "entity.h"
#include "wmath.h"

#ifndef COLLISION_H
#define COLLISION_H

CollisionManifold* Collision_FindCollisionFeatures(Collider* c1, Collider* c2);
CollisionManifold* Collision_FindCollisionFeaturesCircleAndCircle(Circle* a, Circle* b);
CollisionManifold* Collision_FindCollisionFeaturesBoxAndBox(Box* a, Box* b);
CollisionManifold* Collision_FindCollisionFeaturesCircleAndBox(Circle* c, Box* b);
CollisionManifold* Collision_FindCollisionFeaturesBoxAndCircle(Box* b, Circle* c);

#endif
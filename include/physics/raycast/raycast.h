#include <stdbool.h>
#include "primitives.h"
#include "raycastresult.h"
#include "ray.h"

bool Raycast_Rigidbody(Rigidbody* rigidbody, Ray ray, RaycastResult* result);

bool Raycast_Box(Box box, Ray ray, RaycastResult* result);

bool Raycast_Circle(Circle circle, Ray ray, RaycastResult* result);
#include <stdbool.h>
#include "external.h"
#include "entity.h"

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

struct CameraController {

    Component* component;

    vec2* follow;
    vec2 followOffset;

    bool isMoving;
    vec2 initialPos;
    vec2 finalPos;
    float timeCurrent;
    float timeTotal;
    float timeHalf;
    float distance;
    float maxVelocity;

};
typedef struct CameraController CameraController;

Component* CameraController_Init(vec2* follow);

void CameraController_Update(Component* c, float dt);

void CameraController_MoveTo(CameraController* cc, vec2 to, float t);

#endif
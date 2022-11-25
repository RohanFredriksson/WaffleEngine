#include <math.h>
#include <stdbool.h>
#include "external.h"
#include "wmath.h"
#include "window.h"
#include "scene.h"
#include "external.h"
#include "entity.h"
#include "camera.h"

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

struct CameraController {

    Component* component;

    bool isMoving;
    vec2 initialPosition;
    vec2 finalPosition;
    float timeCurrent;
    float timeTotal;
    float timeHalf;
    float distance;
    float maxVelocity;

};
typedef struct CameraController CameraController;

Component* CameraController_Init();

bool CameraController_Load(Component* c, cJSON* json);

void CameraController_MoveTo(CameraController* cc, vec2 to, float t);

#endif
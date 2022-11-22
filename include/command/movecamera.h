#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "command.h"
#include "window.h"
#include "scene.h"
#include "cameracontroller.h"

#ifndef MOVECAMERA_H
#define MOVECAMERA_H

struct MoveCamera {
    Command* command;
    vec2 to;
    float time;
};
typedef struct MoveCamera MoveCamera;

Command* MoveCamera_Init(vec2 to, float time);

void MoveCamera_Execute(Command* a, Component* c);

#endif
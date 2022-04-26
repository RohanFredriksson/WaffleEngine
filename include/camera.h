#include "external.h"

#ifndef CAMERA_H
#define CAMERA_H

struct Camera {
    mat4 projection;
    mat4 view;
    mat4 inverseProjection;
    mat4 inverseView;
    vec2 projectionSize;
    vec2 pos;
    float zoom;
};
typedef struct Camera Camera;

void Camera_Init(Camera* c, vec2 pos);
void Camera_AdjustProjection(Camera* c);
mat4 Camera_GetView(Camera* c);
mat4 Camera_GetProjection(Camera* c);
mat4 Camera_GetInverseView(Camera* c);
mat4 Camera_GetInverseProjection(Camera* c);

#endif
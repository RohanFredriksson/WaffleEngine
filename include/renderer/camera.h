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

void Camera_Init(Camera* c);
void Camera_AdjustProjection(Camera* c);
void Camera_GetView(Camera* c, mat4 matrix);
void Camera_GetProjection(Camera* c, mat4 matrix);
void Camera_GetInverseView(Camera* c, mat4 matrix);
void Camera_GetInverseProjection(Camera* c, mat4 matrix);

float Camera_Left(Camera* c, float x, float pos);
float Camera_Bottom(Camera* c, float y, float pos);

#endif
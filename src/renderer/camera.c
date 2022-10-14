#include "external.h"
#include "camera.h"
#include "window.h"

void Camera_Init(Camera* c) {
    
    glm_mat4_identity(c->projection);
    glm_mat4_identity(c->view);
    glm_mat4_identity(c->inverseProjection);
    glm_mat4_identity(c->inverseView);
    c->pos[0] = 0.0f;
    c->pos[1] = 0.0f;
    c->projectionSize[0] = 12.0f;
    c->projectionSize[1] = 12.0f;
    c->zoom = 1.0f;
    Camera_AdjustProjection(c);

}

void Camera_AdjustProjection(Camera* c) {
    
    c->projectionSize[0] = c->projectionSize[1] * Window_GetAspectRatio();
    glm_ortho(0.0f, c->projectionSize[0] / c->zoom, 0.0f, c->projectionSize[1] / c->zoom, 0.0f, 100.0f, c->projection);
    glm_mat4_inv(c->projection, c->inverseProjection);

}

void Camera_GetView(Camera* c, mat4 matrix) {

    vec3 cameraPos = {c->pos[0] - c->projectionSize[0] / (2 * c->zoom), c->pos[1] - c->projectionSize[1] / (2 * c->zoom), 20.0f};
    vec3 cameraFront = {c->pos[0] - c->projectionSize[0] / (2 * c->zoom), c->pos[1] - c->projectionSize[1] / (2 * c->zoom), -1.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPos, cameraFront, cameraUp, c->view);
    glm_mat4_inv(c->view, c->inverseView);
    
    glm_mat4_copy(c->view, matrix);
}

void Camera_GetProjection(Camera* c, mat4 matrix) {
    glm_mat4_copy(c->projection, matrix);
}

void Camera_GetInverseView(Camera* c, mat4 matrix) {
    mat4 tmp;
    Camera_GetView(c, tmp);
    glm_mat4_copy(c->inverseView, matrix);
}

void Camera_GetInverseProjection(Camera* c, mat4 matrix) {
    glm_mat4_copy(c->inverseProjection, matrix);
}

float Camera_Left(Camera* c, float x, float pos) {
    return x - pos * c->projectionSize[0] * c->zoom;
}

float Camera_Bottom(Camera* c, float y, float pos) {
    return y - pos * c->projectionSize[1] * c->zoom;
}
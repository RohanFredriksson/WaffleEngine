#include "external.h"
#include "camera.h"
#include "window.h"

void Camera_Init(Camera* c) {
    
    glm_mat4_identity(c->projection);
    glm_mat4_identity(c->view);
    glm_mat4_identity(c->inverseProjection);
    glm_mat4_identity(c->inverseView);
    c->pos = { 0.0f, 0.0f };
    c->projectionSize = { 3.0f, 4.0f };
    c->zoom = 1.0f;

}

void Camera_AdjustProjection(Camera* c) {
    
    c->projectionSize[0] = c->projectionSize[1] * Window_GetAspectRatio();
    glm_ortho(0.0f, c->projectionSize[0] / c->zoom, 0.0f, c->projectionSize[1] / c->zoom, 0.0f, 100.0f, c->projection);
    glm_mat4_inv(c->projection, c->inverseProjection);

}

mat4 Camera_GetView(Camera* c) {

    vec3 cameraPos = {c->pos[0] - c->projectionSize[0] / (2 * c->zoom), c->pos[1] - c->projectionSize[1] / (2 * c->zoom), 20.0f};
    vec3 cameraFront = {c->pos[0] - c->projectionSize[0] / (2 * c->zoom), c->pos[1] - c->projectionSize[1] / (2 * c->zoom), -1.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPos, cameraFront, cameraUp, c->view);
    glm_mat4_inv(c->view, c->inverseView);
    
    return c->view;
}

mat Camera_GetProjection(Camera* c) {
    return c->projection;
}

mat4 Camera_GetInverseView(Camera* c) {
    Camera_GetView(c);
    return c->inverseView;
}

mat4 Camera_GetInverseProjection(Camera* c) {
    return c->inverseProjection;
}

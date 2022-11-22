#include "external.h"
#include "camera.h"
#include "window.h"

void Camera_Init(Camera* c) {
    
    glm_mat4_identity(c->projection);
    glm_mat4_identity(c->view);
    glm_mat4_identity(c->inverseProjection);
    glm_mat4_identity(c->inverseView);
    c->position[0] = 0.0f;
    c->position[1] = 0.0f;
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

    vec3 cameraPosition = {c->position[0] - c->projectionSize[0] / (2 * c->zoom), c->position[1] - c->projectionSize[1] / (2 * c->zoom), 20.0f};
    vec3 cameraFront = {c->position[0] - c->projectionSize[0] / (2 * c->zoom), c->position[1] - c->projectionSize[1] / (2 * c->zoom), -1.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    glm_lookat(cameraPosition, cameraFront, cameraUp, c->view);
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

float Camera_Left(Camera* c, float x, float position) {
    return x - position * c->projectionSize[0] * c->zoom;
}

float Camera_Bottom(Camera* c, float y, float position) {
    return y - position * c->projectionSize[1] * c->zoom;
}

cJSON* Camera_Serialise(Camera* c) {

    cJSON* json = cJSON_CreateObject();

    cJSON* projection = cJSON_CreateArray();
    cJSON* view = cJSON_CreateArray();
    for (int i = 0; i < 4; i++) {
        cJSON* projectionRow = cJSON_CreateArray();
        cJSON* viewRow = cJSON_CreateArray();
        for (int j = 0; j < 4; j++) {
            cJSON* projectionValue = cJSON_CreateNumber(c->projection[i][j]);
            cJSON* viewValue = cJSON_CreateNumber(c->view[i][j]);
            cJSON_AddItemToArray(projectionRow, projectionValue);
            cJSON_AddItemToArray(viewRow, viewValue);
        }
        cJSON_AddItemToArray(projection, projectionRow);
        cJSON_AddItemToArray(view, viewRow);
    }
    cJSON_AddItemToObject(json, "projection", projection);
    cJSON_AddItemToObject(json, "view", view);

    cJSON* position = cJSON_CreateArray();
    cJSON* projectionSize = cJSON_CreateArray();
    for (int i = 0; i < 2; i++) {
        cJSON* positionValue = cJSON_CreateNumber(c->position[i]);
        cJSON* projectionSizeValue = cJSON_CreateNumber(c->projectionSize[i]);
        cJSON_AddItemToArray(position, positionValue);
        cJSON_AddItemToArray(projectionSize, projectionSizeValue);
    }
    cJSON_AddItemToObject(json, "position", position);
    cJSON_AddItemToObject(json, "projectionSize", projectionSize);

    return json;

}
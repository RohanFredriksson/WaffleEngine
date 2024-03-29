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
    WIO_AddVec2(json, "position", c->position);
    WIO_AddVec2(json, "projectionSize", c->projectionSize);
    WIO_AddFloat(json, "zoom", c->zoom);
    return json;

}

bool Camera_Load(Camera* c, cJSON* json) {

    // Load the position
    if (!WIO_ParseVec2(json, "position", c->position)) {
        printf("ERROR::CAMERA::LOAD::JSON_PARSE_ERROR: \"position\" attribute could not be parsed.\n");
        return 0;    
    }

    // Load the projection size
    if (!WIO_ParseVec2(json, "projectionSize", c->projectionSize)) {
        printf("ERROR::CAMERA::LOAD::JSON_PARSE_ERROR: \"projectionSize\" attribute could not be parsed.\n");
        return 0;    
    }

    // Load the zoom
    if (!WIO_ParseFloat(json, "zoom", &c->zoom)) {
        printf("ERROR::CAMERA::LOAD::JSON_PARSE_ERROR: \"zoom\" attribute could not be parsed.\n");
        return 0;    
    }

    // Initialise the matrices
    glm_mat4_identity(c->projection);
    glm_mat4_identity(c->view);
    glm_mat4_identity(c->inverseProjection);
    glm_mat4_identity(c->inverseView);
    Camera_AdjustProjection(c);

    // Compute the inverse matrices.
    mat4 tmp;
    Camera_GetInverseView(c, tmp);

    return 1;

}
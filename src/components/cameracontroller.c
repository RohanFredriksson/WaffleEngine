
#include "cameracontroller.h"

Component* CameraController_Init() {

    Component* c = Component_Init("CameraController", &CameraController_Update, NULL, &CameraController_Serialise, NULL);

    // Allocate some memory for the camera controller.
    CameraController* cc = malloc(sizeof(CameraController));

    // Initialise the camera controller.
    cc->component = c;
    cc->isMoving = 0;

    // Attach the camera controller to the component
    c->data = cc;
    return c;

}

void CameraController_Update(Component* c, float dt) {

    CameraController* cc = (CameraController*) c->data;
    Camera* camera = &Window_GetScene()->camera;

    // If the camera is moving, smoothly move the camera.
    if (cc->isMoving) {

        // If the camera has moved to the end, reset all movement variables, 
        // and set the position exactly
        if (cc->timeCurrent >= cc->timeTotal) {
            cc->isMoving = 0;
            glm_vec2_copy(cc->finalPosition, camera->position);
        }

        // If the camera is still moving, calculate its new position.
        else {

            // Calculate the current velocity of coefficient.
            float velocityFactor = sqrtf(1 - powf((cc->timeCurrent - cc->timeHalf) / cc->timeHalf, 2));

            // Calculate how much to displace the camera.
            vec2 currentDisplacement;
            glm_vec2_copy(cc->finalPosition, currentDisplacement);
            glm_vec2_sub(currentDisplacement, cc->initialPosition, currentDisplacement);
            glm_vec2_normalize(currentDisplacement);
            glm_vec2_scale(currentDisplacement, cc->maxVelocity * velocityFactor * dt, currentDisplacement);

            // Add the displacement to the camera.
            glm_vec2_add(camera->position, currentDisplacement, camera->position);

        }

        cc->timeCurrent = cc->timeCurrent + dt;

    }

}

cJSON* CameraController_Serialise(Component* c) {

    CameraController* cc = (CameraController*) c->data;

    cJSON* json = cJSON_CreateObject();

    cJSON* isMoving = cJSON_CreateBool(cc->isMoving);
    cJSON_AddItemToObject(json, "isMoving", isMoving);

    cJSON* initialPosition = cJSON_CreateArray();
    cJSON* xInitial = cJSON_CreateNumber((double) cc->initialPosition[0]);
    cJSON* yInitial = cJSON_CreateNumber((double) cc->initialPosition[1]);
    cJSON_AddItemToArray(initialPosition, xInitial);
    cJSON_AddItemToArray(initialPosition, yInitial);
    cJSON_AddItemToObject(json, "initialPosition", initialPosition);

    cJSON* finalPosition = cJSON_CreateArray();
    cJSON* xFinal = cJSON_CreateNumber((double) cc->finalPosition[0]);
    cJSON* yFinal = cJSON_CreateNumber((double) cc->finalPosition[1]);
    cJSON_AddItemToArray(finalPosition, xFinal);
    cJSON_AddItemToArray(finalPosition, yFinal);
    cJSON_AddItemToObject(json, "finalPosition", finalPosition);

    cJSON* timeCurrent = cJSON_CreateNumber((double) cc->timeCurrent);
    cJSON_AddItemToObject(json, "timeCurrent", timeCurrent);

    cJSON* timeTotal = cJSON_CreateNumber((double) cc->timeTotal);
    cJSON_AddItemToObject(json, "timeTotal", timeTotal);

    cJSON* timeHalf = cJSON_CreateNumber((double) cc->timeHalf);
    cJSON_AddItemToObject(json, "timeHalf", timeHalf);

    cJSON* distance = cJSON_CreateNumber((double) cc->distance);
    cJSON_AddItemToObject(json, "distance", distance);

    cJSON* maxVelocity = cJSON_CreateNumber((double) cc->maxVelocity);
    cJSON_AddItemToObject(json, "maxVelocity", maxVelocity);

    return json;

}

void CameraController_MoveTo(CameraController* cc, vec2 to, float t) {

    Camera* camera = &Window_GetScene()->camera;

    glm_vec2_copy(camera->position, cc->initialPosition);
    glm_vec2_copy(to, cc->finalPosition);
    cc->distance = glm_vec2_distance(cc->initialPosition, cc->finalPosition);

    // If the camera, doesn't need to move, don't move it.
    if (cc->distance == 0) {
        return;
    }

    cc->timeCurrent = 0;
    cc->timeTotal = t;
    cc->timeHalf = t/2;

    // If the time required to move is zero, move the camera immediately.
    if (cc->timeTotal <= 0) {
        glm_vec2_copy(to, camera->position);
        return;
    }

    // Define max velocity, and raise the moving flag.
    cc->maxVelocity = (2 * cc->distance) / (GLM_PI * cc->timeHalf);
    cc->isMoving = 1;

}
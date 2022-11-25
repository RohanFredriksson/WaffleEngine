
#include "cameracontroller.h"

static void CameraController_Update(Component* c, float dt) {

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

static cJSON* CameraController_Serialise(Component* c) {

    CameraController* cc = (CameraController*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddBool(json, "isMoving", cc->isMoving);
    WIO_AddVec2(json, "initialPosition", cc->initialPosition);
    WIO_AddVec2(json, "finalPosition", cc->finalPosition);
    WIO_AddFloat(json, "timeCurrent", cc->timeCurrent);
    WIO_AddFloat(json, "timeTotal", cc->timeTotal);
    WIO_AddFloat(json, "timeHalf", cc->timeHalf);
    WIO_AddFloat(json, "distance", cc->distance);
    WIO_AddFloat(json, "maxVelocity", cc->maxVelocity);
    return json;

}

static CameraController* _CameraController_Init(Component* c,
                                                bool isMoving, 
                                                vec2 initialPosition, 
                                                vec2 finalPosition, 
                                                float timeCurrent, 
                                                float timeTotal, 
                                                float timeHalf,
                                                float distance, 
                                                float maxVelocity) {

    CameraController* cc = malloc(sizeof(CameraController));
    cc->component = c;
    cc->isMoving = isMoving;
    glm_vec2_copy(initialPosition, cc->initialPosition);
    glm_vec2_copy(finalPosition, cc->finalPosition);
    cc->timeCurrent = timeCurrent;
    cc->timeTotal = timeTotal;
    cc->timeHalf = timeHalf;
    cc->distance = distance;
    cc->maxVelocity = maxVelocity;

    c->update = &CameraController_Update;
    c->serialise = &CameraController_Serialise;
    c->data = cc;     

    return cc;                                 
}

Component* CameraController_Init() {
    Component* c = Component_Init("CameraController");
    _CameraController_Init(c, 0, (vec2) {0,0}, (vec2) {0,0}, 0, 0, 0, 0, 0);
    return c;
}

bool CameraController_Load(Component* c, cJSON* json) {

    bool isMoving;
    vec2 initialPosition;
    vec2 finalPosition;
    float timeCurrent;
    float timeTotal;
    float timeHalf;
    float distance;
    float maxVelocity;

    if (!WIO_ParseBool(json, "isMoving", &isMoving)) {return 0;}
    if (!WIO_ParseVec2(json, "initialPosition", initialPosition)) {return 0;}
    if (!WIO_ParseVec2(json, "finalPosition", finalPosition)) {return 0;}
    if (!WIO_ParseFloat(json, "timeCurrent", &timeCurrent)) {return 0;}
    if (!WIO_ParseFloat(json, "timeTotal", &timeTotal)) {return 0;}
    if (!WIO_ParseFloat(json, "timeHalf", &timeHalf)) {return 0;}
    if (!WIO_ParseFloat(json, "distance", &distance)) {return 0;}
    if (!WIO_ParseFloat(json, "maxVelocity", &maxVelocity)) {return 0;}

    // Initialise the camera controller class.
    _CameraController_Init(c, isMoving, initialPosition, finalPosition, timeCurrent, timeTotal, timeHalf, distance, maxVelocity);

    return 1;
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
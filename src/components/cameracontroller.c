#include <math.h>
#include "external.h"
#include "util.h"
#include "window.h"
#include "scene.h"
#include "cameracontroller.h"
#include "gameobject.h"
#include "camera.h"

Component* CameraController_Init(Transform* follow) {

    Component* c = Component_Init("CameraController", &CameraController_Update, &CameraController_Free);

    // Allocate some memory for the camera controller.
    CameraController* cc = malloc(sizeof(CameraController));

    // Initialise the camera controller.
    cc->component = c;

    cc->isMoving = 0;
    cc->follow = follow;

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
            glm_vec2_copy(cc->finalPos, camera->pos);
        }

        // If the camera is still moving, calculate its new position.
        else {

            // Calculate the current velocity of coefficient.
            float velocityFactor = sqrtf(1 - powf((cc->timeCurrent - cc->timeHalf) / cc->timeHalf, 2));

            // Calculate how much to displace the camera.
            vec2 currentDisplacement;
            glm_vec2_copy(cc->finalPos, currentDisplacement);
            glm_vec2_sub(currentDisplacement, cc->initialPos, currentDisplacement);
            glm_vec2_normalize(currentDisplacement);
            glm_vec2_scale(currentDisplacement, cc->maxVelocity * velocityFactor * dt, currentDisplacement);

            // Add the displacement to the camera.
            glm_vec2_add(camera->pos, currentDisplacement, camera->pos);

        }

        cc->timeCurrent = cc->timeCurrent + dt;

    }

}

void CameraController_Free(Component* c) {
    Component_Free(c);
    free(c->data);
}

void CameraController_MoveTo(CameraController* cc, float x, float y, float t) {

    Camera* camera = &Window_GetScene()->camera;

    glm_vec2_copy(camera->pos, cc->initialPos);
    cc->finalPos[0] = x;
    cc->finalPos[1] = y;
    cc->distance = glm_vec2_distance(cc->initialPos, cc->finalPos);

    // If the camera, doesn't need to move, don't move it.
    if (cc->distance == 0) {
        return;
    }

    cc->timeCurrent = 0;
    cc->timeTotal = t;
    cc->timeHalf = t/2;

    // If the time required to move is zero, move the camera immediately.
    if (cc->timeTotal <= 0) {
        camera->pos[0] = x;
        camera->pos[1] = y;
        return;
    }

    // Define max velocity, and raise the moving flag.
    cc->maxVelocity = (2 * cc->distance) / (M_PI * cc->timeHalf);
    cc->isMoving = 1;

}
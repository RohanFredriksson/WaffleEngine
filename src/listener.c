#include <stdio.h>

#include "external.h"
#include "window.h"
#include "listener.h"
#include "camera.h"

bool KeyListener_KeyPressed[350] = {0};
bool KeyListener_KeyBeginPress[350] = {0};

void KeyListener_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        KeyListener_KeyPressed[key] = 1;
        KeyListener_KeyBeginPress[key] = 1;
    }

    else if (action == GLFW_RELEASE) {
        KeyListener_KeyPressed[key] = 0;
        KeyListener_KeyBeginPress[key] = 0;
    }

}

bool KeyListener_IsKeyPressed(int key) {
    return KeyListener_KeyPressed[key];
}

bool KeyListener_IsBeginPress(int key) {
    
    bool result = KeyListener_KeyBeginPress[key];
    if (result) {
        KeyListener_KeyBeginPress[key] = 0;
    }

    return result;

}

void WindowListener_ResizeCallback(GLFWwindow* window, int screenWidth, int screenHeight) {

    Window_SetWidth(screenWidth);
    Window_SetHeight(screenHeight);
    Camera_AdjustProjection(&Window_GetScene()->camera);
    glViewport(0, 0, screenWidth, screenHeight);

}

double MouseListener_ScrollX = 0.0;
double MouseListener_ScrollY = 0.0;
double MouseListener_X = 0.0; 
double MouseListener_Y = 0.0; 
double MouseListener_LastX = 0.0; 
double MouseListener_LastY = 0.0; 
double MouseListener_WorldX = 0.0; 
double MouseListener_WorldY = 0.0; 
double MouseListener_LastWorldX = 0.0; 
double MouseListener_LastWorldY = 0.0;
bool MouseListener_MouseButtonPressed[9] = {0};
bool MouseListener_IsDragging = 0;
int MouseListener_MouseButtonsDown = 0;

void MouseListener_CalcOrthoX() {

    float currentX = (float)MouseListener_X - Window_GetWidth();
    currentX = (currentX / Window_GetWidth()) * 2.0f - 1.0f;
    vec4 tmp = { currentX, 0, 0, 1};

    Camera* camera =  &(Window_GetScene()->camera);
    mat4 inverseView;
    mat4 inverseProjection;
    mat4 viewProjection;
    
    Camera_GetInverseView(camera, inverseView);
    Camera_GetInverseProjection(camera, inverseProjection);
    glm_mat4_mul(inverseView, inverseProjection, viewProjection);
    glm_mat4_mulv(viewProjection, tmp, tmp);

    MouseListener_WorldX = tmp[0];

}

void MouseListener_CalcOrthoY() {

    float currentY = (float)MouseListener_Y - Window_GetHeight();
    currentY = (currentY / Window_GetHeight()) * 2.0f - 1.0f;
    vec4 tmp = { 0, currentY, 0, 1};

    Camera* camera =  &(Window_GetScene()->camera);
    mat4 inverseView;
    mat4 inverseProjection;
    mat4 viewProjection;

    Camera_GetInverseView(camera, inverseView);
    Camera_GetInverseProjection(camera, inverseProjection);

    glm_mat4_mul(inverseView, inverseProjection, viewProjection);
    glm_mat4_mulv(viewProjection, tmp, tmp);

    MouseListener_WorldY = tmp[1];

}

void MouseListener_MousePosCallback(GLFWwindow* window, double xPos, double yPos) {

    if (MouseListener_MouseButtonsDown > 0) {
        MouseListener_IsDragging = 1;
    }

    MouseListener_LastX = MouseListener_X;
    MouseListener_LastY = MouseListener_Y;
    MouseListener_LastWorldX = MouseListener_WorldX;
    MouseListener_LastWorldY = MouseListener_WorldY;
    MouseListener_X = xPos;
    MouseListener_Y = yPos;
    MouseListener_CalcOrthoX();
    MouseListener_CalcOrthoY();

}

void MouseListener_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (action == GLFW_PRESS) {

        MouseListener_MouseButtonsDown++;

        if (button < 9) {
            MouseListener_MouseButtonPressed[button] = 1;
        }

    }

    else if (action == GLFW_RELEASE) {

        MouseListener_MouseButtonsDown--;

        if (button < 9) {
            MouseListener_MouseButtonPressed[button] = 0;
            MouseListener_IsDragging = 0;
        }

    }

}

void MouseListener_MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {

    MouseListener_ScrollX = xOffset;
    MouseListener_ScrollY = yOffset;

}

void MouseListener_EndFrame() {

    MouseListener_ScrollX = 0.0;
    MouseListener_ScrollY = 0.0;
    MouseListener_LastX = MouseListener_X;
    MouseListener_LastY = MouseListener_Y;
    MouseListener_LastWorldX = MouseListener_WorldX;
    MouseListener_LastWorldY = MouseListener_WorldY;
    MouseListener_CalcOrthoX();
    MouseListener_CalcOrthoY();

}
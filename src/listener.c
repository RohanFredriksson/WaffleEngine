#include <stdio.h>

#include "external.h"
#include "window.h"
#include "listener.h"

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
    glViewport(0, 0, screenWidth, screenHeight);

}

/*
double MouseListener_ScrollX = 0.0, 
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

}

void MouseListener_CalcOrthoY() {

}

void MouseListener_MousePosCallback(GLFWwindow* window, double xPos, double yPos) {

}

void MouseListener_MouseButtonCallback(GLFWwindow* window, double xPos, double yPos) {

}

void MouseListener_MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {

}

void MouseListener_EndFrame() {

}
*/
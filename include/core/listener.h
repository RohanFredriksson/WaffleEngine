#include <stdbool.h>
#include "external.h"

#ifndef LISTENER_H
#define LISTENER_H

void KeyListener_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool KeyListener_IsKeyPressed(int key);
bool KeyListener_IsBeginPress(int key);

void WindowListener_ResizeCallback(GLFWwindow* window, int screenWidth, int screenHeight);

void MouseListener_CalcOrthoX();
void MouseListener_CalcOrthoY();
void MouseListener_MousePosCallback(GLFWwindow* window, double xPos, double yPos);
void MouseListener_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MouseListener_MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
bool MouseListener_MouseButtonDown(int button);
bool MouseListener_MouseButtonBeginDown(int button);
bool MouseListener_MouseDragging();
double MouseListener_GetX();
double MouseListener_GetY();
double MouseListener_GetDx();
double MouseListener_GetDy();
double MouseListener_GetWorldX();
double MouseListener_GetWorldY();
double MouseListener_GetWorldDx();
double MouseListener_GetWorldDy();
double MouseListener_GetScrollX();
double MouseListener_GetScrollY();
void MouseListener_EndFrame();

#endif
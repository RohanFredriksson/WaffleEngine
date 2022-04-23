#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#ifndef LISTENER_H
#define LISTENER_H

bool KeyListener_KeyPressed[350];
bool KeyListener_KeyBeginPress[350];

void KeyListener_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool KeyListener_IsKeyPressed(int key);
bool KeyListener_IsBeginPress(int key);

void WindowListener_ResizeCallback(GLFWwindow* window, int screenWidth, int screenHeight);

/*
double MouseListener_ScrollX, MouseListener_ScrollY;
double MouseListener_X, MouseListener_Y, MouseListener_LastX, MouseListener_LastY, MouseListener_WorldX, MouseListener_WorldY, MouseListener_LastWorldX, MouseListener_LastWorldY;
bool MouseListener_MouseButtonPressed[9];
bool MouseListener_IsDragging;
int MouseListener_MouseButtonsDown;

void MouseListener_CalcOrthoX();
void MouseListener_CalcOrthoY();
void MouseListener_MousePosCallback(GLFWwindow* window, double xPos, double yPos);
void MouseListener_MouseButtonCallback(GLFWwindow* window, double xPos, double yPos);
void MouseListener_MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MouseListener_EndFrame();
*/

#endif
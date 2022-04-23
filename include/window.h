#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#ifndef WINDOW_H
#define WINDOW_H

GLFWwindow* window;
ivec2 windowSize;
float fps;

void Window_Run();
int Window_Init();
void Window_Loop();
void Window_Exit();

int Window_GetWidth();
int Window_GetHeight();
void Window_SetWidth(int width);
void Window_SetHeight(int height);
float Window_GetAspectRatio();

#endif
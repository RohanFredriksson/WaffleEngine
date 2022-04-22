#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#ifndef WINDOW_H
#define WINDOW_H

GLFWwindow* window;

int width, height;
vec2 viewportPos;
vec2 viewportSize;
float fps;

void Window_Run();
int Window_Init();
void Window_Loop();
void Window_Exit();

#endif
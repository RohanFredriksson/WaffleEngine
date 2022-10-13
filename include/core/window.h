#include "external.h"
#include "scene.h"

#ifndef WINDOW_H
#define WINDOW_H

Scene scene;
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
Scene* Window_GetScene();

void Window_SetWindowed();
void Window_SetFullscreen();
void Window_SetFullscreenWindowed();
GLFWmonitor* Window_GetCurrentMonitor();

void Window_ResetFramebuffers();
int Window_ReadPixel(int x, int y);

#endif
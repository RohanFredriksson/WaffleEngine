#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef WINDOW_H
#define WINDOW_H

GLFWwindow* window;
int width, height;
float fps;

void window_run();
int window_init();
void window_loop();
void window_exit();

#endif
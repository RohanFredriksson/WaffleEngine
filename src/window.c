#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "window.h"

int width = 800;
int height = 800;
vec2 viewportPos = { 0.0f, 0.0f };
vec2 viewportSize = { 0.0f, 0.0f };
float fps = -1.0f;

int Window_Init() {

    // Initialise GLFW
    if (!glfwInit()) {
		printf("Unable to initialise GLFW.\n");
		return 1;
	}

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    window = glfwCreateWindow(width, height, "Waffle Engine", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        return 1;
    }

    // Make the OpenGl context current
	glfwMakeContextCurrent(window);

    //Enable v-sync
    glfwSwapInterval(1);

    //Load GLAD so it configures OpenGL
	gladLoadGL();
    return 0;

}

void Window_Loop() {

    float beginTime = (float)glfwGetTime();
    float endTime = (float)glfwGetTime();
    float dt = -1.0f;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);

        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;
        fps = 1/dt;

    }

}

void Window_Exit() {

    // Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

}

void Window_Run() {

    if (Window_Init()) {return;}
    Window_Loop();
    Window_Exit();

}
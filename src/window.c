#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

int window_init() {

    width = 800;
    height = 800;
    fps = -1.0f;

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

void window_loop() {

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

void window_exit() {

    // Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

}

void window_run() {

    if (window_init()) {return;}
    window_loop();
    window_exit();

}
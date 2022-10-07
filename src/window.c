#include <stdio.h>

#include "external.h"
#include "window.h"
#include "listener.h"
#include "scene.h"
#include "title.h"
#include "assetpool.h"

#include "texture.h"
#include "framebuffer.h"

Scene scene;
ivec2 windowSize = { 800, 800 };
float fps = -1.0f;

int Window_Init() {

    // Initialise GLFW
    if (!glfwInit()) {
		printf("ERROR::WINDOW::GLFW_INITIALIZATION_FAILED\n");
		return 1;
	}

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    window = glfwCreateWindow(windowSize[0], windowSize[1], "Waffle Engine", NULL, NULL);
    if (window == NULL) {
        printf("ERROR::WINDOW::GLFW_WINDOW_CREATION_FAILED\n");
        return 1;
    }

    // Manage callbacks
	glfwSetKeyCallback(window, KeyListener_KeyCallback);
	glfwSetWindowSizeCallback(window, WindowListener_ResizeCallback);
	glfwSetCursorPosCallback(window, MouseListener_MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseListener_MouseButtonCallback);
	glfwSetScrollCallback(window, MouseListener_MouseScrollCallback);

    // Make the OpenGl context current
	glfwMakeContextCurrent(window);

    //Enable v-sync
    glfwSwapInterval(1);

    //Load GLAD so it configures OpenGL
	gladLoadGL();

    // Set up the current scene.
    Scene_Init(&scene, Title_Init);
    Scene_Start(&scene);

    return 0;

}

void Window_Loop() {

    float beginTime = (float)glfwGetTime();
    float endTime = (float)glfwGetTime();
    float dt = -1.0f;

    Shader* shader = ShaderPool_Get("./assets/shaders/default.vert", "./assets/shaders/default.frag");

    FrameBuffer fb;
    FrameBuffer_Init(&fb, windowSize[0], windowSize[1]);

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (dt > 0) {

            Scene_Update(&scene, dt);

            FrameBuffer_Bind(&fb);
            Renderer_BindShader(shader);
            Scene_Render(&scene);
            FrameBuffer_Unbind(&fb);
            
            Scene_Render(&scene);
        }

        glfwSwapBuffers(window);
        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;
        fps = 1/dt;

    }

    FrameBuffer_Free(&fb);

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

int Window_GetWidth() {
    return windowSize[0];
}

int Window_GetHeight() {
    return windowSize[1];
}

void Window_SetWidth(int width) {
    windowSize[0] = width;
}

void Window_SetHeight(int height) {
    windowSize[1] = height;
}

float Window_GetAspectRatio() {
    return (float) windowSize[0] / (float) windowSize[1];
}

Scene* Window_GetScene() {
    return &scene;
}
#include <stdio.h>

#include "external.h"
#include "window.h"
#include "listener.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"

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
	//glfwSetCursorPosCallback(window, MouseListener::mousePosCallback);
	//glfwSetMouseButtonCallback(window, MouseListener::mouseButtonCallback);
	//glfwSetScrollCallback(window, MouseListener::mouseScrollCallback);

    // Make the OpenGl context current
	glfwMakeContextCurrent(window);

    //Enable v-sync
    glfwSwapInterval(1);

    //Load GLAD so it configures OpenGL
	gladLoadGL();

    // Set up the current scene.
    Scene_Init(&scene);

    return 0;

}

void Window_Loop() {

    float beginTime = (float)glfwGetTime();
    float endTime = (float)glfwGetTime();
    float dt = -1.0f;

    /*
    Shader s;
    Shader_Init(&s, "./assets/shaders/default.vert", "./assets/shaders/default.frag");
    Shader_Compile(&s);
    Shader_Use(&s);

    Texture t;
    Texture_Init(&t, "./assets/textures/armaan.png");
    Texture_Bind(&t);

    float vertices[] = {
        // Position    Colour                    Tex Coords    Tex ID
        0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   (float)t.id, // top right
        0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   (float)t.id, // bottom right
       -0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,   (float)t.id, // bottom left
       -0.5f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,   (float)t.id  // top left 
    };

    int indices[] = {2, 3, 0, 0, 2, 1};
    int textures[] = {t.id};
    Shader_UploadIntArray(&s, "uTextures", textures);
    
    int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    // Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Introduce the indices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(2 * sizeof(float)));
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(1);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(2);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(3);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Bind the EBO to 0 so that we don't accidentally modify it
	// MAKE SURE TO UNBIND IT AFTER UNBINDING THE VAO, as the EBO is linked in the VAO
	// This does not apply to the VBO because the VBO is already linked to the VAO during glVertexAttribPointer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    */

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        if (dt > 0) {
            Scene_Update(&scene, dt);
            Scene_Render(&scene);
        }

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
#include <stdio.h>

#include "external.h"
#include "window.h"
#include "debugdraw.h"
#include "cameracontroller.h"
#include "font.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

GLFWwindow* window;
Scene scene;

struct ImGuiContext* ctx;
struct ImGuiIO* io;

FrameBuffer entityTexture;
Shader* defaultShader;
Shader* entityShader;

ivec2 windowSize = { 1366, 768 };
float fps = -1.0f;

int Window_Init() {

    // Initialise GLFW
    if (!glfwInit()) {
		printf("ERROR::WINDOW::GLFW_INITIALIZATION_FAILED\n");
		return 1;
	}

    // Configure GLFW
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

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

    // Enable v-sync
    glfwSwapInterval(1);

    // Make the window visible
    glfwShowWindow(window);

    // Load GLAD so it configures OpenGL
	gladLoadGL();

    // Load the asset pool
    AssetPool_Init();

    // Initialise cimgui
    ctx = igCreateContext(NULL);
    io  = igGetIO();

    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    igStyleColorsDark(NULL);

    // Set the window configuration.
    //Window_SetFullscreenWindowed();

    // Enable alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // Set up the framebuffers.
    PickingTexture_Init(&entityTexture, windowSize[0], windowSize[1]);

    // Set up the shaders.
    defaultShader = ShaderPool_Get("./assets/shaders/default.vert", "./assets/shaders/default.frag");
    entityShader = ShaderPool_Get("./assets/shaders/default.vert", "./assets/shaders/entity.frag");

    // Set up the current scene.
    Scene_Init(&scene, "title", Title_Init);
    //Scene_Load(&scene, "title");

    DebugDraw_Init();
    DebugDraw_Start();

    return 0;

}

void Window_Loop() {

    float beginTime = (float)glfwGetTime();
    float endTime = (float)glfwGetTime();
    float dt = -1.0f;

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (dt > 0) {

            Scene_Update(&scene, dt);

            if (MouseListener_MouseDragging()) {
                if (MouseListener_GetDx() != 0) {scene.camera.position[0] = scene.camera.position[0] - MouseListener_GetWorldDx();}
                if (MouseListener_GetDy() != 0) {scene.camera.position[1] = scene.camera.position[1] - MouseListener_GetWorldDy();}
            }

            if (MouseListener_GetScrollY() != 0) {

                if (MouseListener_GetScrollY() > 0) {
                    scene.camera.zoom = scene.camera.zoom * 1.1f;
                    scene.camera.position[0] = scene.camera.position[0] + (MouseListener_GetWorldX() - scene.camera.position[0]) * 0.0909090909f;
                    scene.camera.position[1] = scene.camera.position[1] + (MouseListener_GetWorldY() - scene.camera.position[1]) * 0.0909090909f;
                }

                else {
                    scene.camera.zoom = scene.camera.zoom / 1.1f;
                    scene.camera.position[0] = scene.camera.position[0] - (MouseListener_GetWorldX() - scene.camera.position[0]) * 0.1f;
                    scene.camera.position[1] = scene.camera.position[1] - (MouseListener_GetWorldY() - scene.camera.position[1]) * 0.1f;
                }

                Camera_AdjustProjection(&scene.camera);

            }

            //----
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            igNewFrame();
            igShowDemoWindow(NULL);
            //----
            
            glDisable(GL_BLEND);
            FrameBuffer_Bind(&entityTexture);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Renderer_BindShader(entityShader);
            Scene_Render(&scene);
            FrameBuffer_Unbind(&entityTexture);
            glEnable(GL_BLEND);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            Renderer_BindShader(defaultShader);
            Scene_Render(&scene);

            DebugDraw_BeginFrame();
            DebugDraw_Draw();

            // Render the imgui
            igRender();
            ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

        }

        glfwSwapBuffers(window);
        MouseListener_EndFrame();
        Window_ReadPixel(0, 0); // Read now to stop a pipeline stall.
        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;
        fps = 1/dt;

    }

}

void Window_Exit() {

    Scene_Save(&scene);

    DebugDraw_Free();

    // Free the framebuffers
    FrameBuffer_Free(&entityTexture);

    // Terminate imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(ctx);

    // Delete window before ending the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending the program
	glfwTerminate();

    // Free the asset pool.
    AssetPool_Free();

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

void Window_SetWindowed() {

    const GLFWvidmode* mode = glfwGetVideoMode(Window_GetCurrentMonitor());
    int windowWidth = (int)((float)mode->width / 2.25f);
    int windowHeight = (int)((float)mode->height / 2.25f);
    
    // Set window attributes
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
    glfwRestoreWindow(window);
    
    // Set the window size
    glfwSetWindowSize(window, windowWidth, windowHeight);

    // Set the window position.
    int x, y;
    glfwGetMonitorPos(Window_GetCurrentMonitor(), &x, &y);
    glfwSetWindowPos(window, x + (mode->width - windowWidth) / 2, y + (mode->height - windowHeight) / 2);

}

void Window_SetFullscreen() {

    // Set the window attributes.
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    glfwMaximizeWindow(window);

    // Set the window position.
    int x, y;
    glfwGetMonitorPos(Window_GetCurrentMonitor(window), &x, &y);
    glfwSetWindowPos(window, x, y);

    // Set the window size.
    const GLFWvidmode* mode = glfwGetVideoMode(Window_GetCurrentMonitor());
    glfwSetWindowSize(window, mode->width, mode->height);

}

void Window_SetFullscreenWindowed() {

    // Set the window attributes.
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowAttrib(window, GLFW_MAXIMIZED, GLFW_FALSE);
    glfwRestoreWindow(window);

    // Set the window position.
    int x, y;
    glfwGetMonitorPos(Window_GetCurrentMonitor(window), &x, &y);
    glfwSetWindowPos(window, x, y);

    // Set the window size.
    const GLFWvidmode* mode = glfwGetVideoMode(Window_GetCurrentMonitor());
    glfwSetWindowSize(window, mode->width, mode->height);

}

GLFWmonitor* Window_GetCurrentMonitor() {

    int wx, wy, ww, wh;
    int mx, my, mw, mh;

    int overlap, bestOverlap;
    GLFWmonitor* bestMonitor;
    GLFWmonitor** monitors;
    int numMonitors;

    bestOverlap = 0;
    bestMonitor = glfwGetPrimaryMonitor();

    glfwGetWindowPos(window, &wx, &wy);
    glfwGetWindowSize(window, &ww, &wh);
    monitors = glfwGetMonitors(&numMonitors);

    for (int i = 0; i < numMonitors; i++) {
        
        GLFWmonitor* monitor = monitors[i];
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwGetMonitorPos(monitor, &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap = WMath_MaxInt(0, WMath_MinInt(wx + ww, mx + mw) - WMath_MaxInt(wx, mx)) * 
                  WMath_MaxInt(0, WMath_MinInt(wy + wh, my + mh) - WMath_MaxInt(wy, my));

        if (bestOverlap < overlap) {
            bestOverlap = overlap;
            bestMonitor = monitor;
        }
    }

    return bestMonitor;
}

void Window_ResetFramebuffers() {
    FrameBuffer_Free(&entityTexture);
    PickingTexture_Init(&entityTexture, windowSize[0], windowSize[1]);
}

int Window_ReadPixel(int x, int y) {

    // Create a buffer to store pixel data.
    float pixel;

    // Bind the framebuffer and read the corresponding pixel.
    FrameBuffer_Bind(&entityTexture);
    glReadPixels(x, windowSize[1]-y-1, 1, 1, GL_RED, GL_FLOAT, &pixel);
    FrameBuffer_Unbind(&entityTexture);

    // Return the value of this pixel.
    return (int)pixel - 1;

}
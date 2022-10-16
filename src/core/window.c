#include <stdio.h>

#include "external.h"
#include "window.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

#include "cameracontroller.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

GLFWwindow* window;
Scene scene;

/*
struct nk_glfw glfw = {0};
struct nk_context* ctx;
struct nk_colorf bg;
*/

FrameBuffer entityTexture;
Shader* defaultShader;
Shader* entityShader;

ivec2 windowSize = { 800, 800 };
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

    //Load GLAD so it configures OpenGL
	gladLoadGL();

    /*
    ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);
    */

    // Set the window configuration.
    //Window_SetFullscreenWindowed();

    // Set up the framebuffers.
    PickingTexture_Init(&entityTexture, windowSize[0], windowSize[1]);

    // Set up the shaders.
    defaultShader = ShaderPool_Get("./assets/shaders/default.vert", "./assets/shaders/default.frag");
    entityShader = ShaderPool_Get("./assets/shaders/default.vert", "./assets/shaders/entity.frag");

    // Set up the current scene.
    Scene_Init(&scene, Title_Init);
    Scene_Start(&scene);

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
            
            FrameBuffer_Bind(&entityTexture);
            Renderer_BindShader(entityShader);
            Scene_Render(&scene);
            FrameBuffer_Unbind(&entityTexture);

            if (MouseListener_MouseButtonBeginDown(GLFW_MOUSE_BUTTON_LEFT)) {
                
                int x = MouseListener_GetX();
                int y = MouseListener_GetY();
                int id = Window_ReadPixel(x, y);
                
                GameObject* go = Scene_GetGameObjectByID(&scene, id);
                if (go != NULL) {
                    
                    Component* c = GameObject_GetComponent(go, "CameraController");
                    if (c != NULL) {
                        CameraController* cc = (CameraController*) c->data;
                        CameraController_MoveTo(cc, 1.0f, -1.5f, 1.0f);
                    }

                }

            }
            
            Renderer_BindShader(defaultShader);
            Scene_Render(&scene);

            /*
            nk_glfw3_new_frame(&glfw);
            if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
                
                enum {EASY, HARD};
                static int op = EASY;
                static int property = 20;
                nk_layout_row_static(ctx, 30, 80, 1);
                if (nk_button_label(ctx, "button")) {
                    fprintf(stdout, "button pressed\n");
                }
                    
                nk_layout_row_dynamic(ctx, 30, 2);
                if (nk_option_label(ctx, "easy", op == EASY)) {op = EASY;}
                if (nk_option_label(ctx, "hard", op == HARD)) {op = HARD;}

                nk_layout_row_dynamic(ctx, 25, 1);
                nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "background:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 25, 1);
                if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
                    nk_layout_row_dynamic(ctx, 120, 1);
                    bg = nk_color_picker(ctx, bg, NK_RGBA);
                    nk_layout_row_dynamic(ctx, 25, 1);
                    bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                    bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                    bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                    bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                    nk_combo_end(ctx);
                }
            }
            nk_end(ctx);
            nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
            */

        }

        glfwSwapBuffers(window);
        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;
        fps = 1/dt;

    }

}

void Window_Exit() {

    // Free the framebuffers
    FrameBuffer_Free(&entityTexture);

    // Delete window before ending the program
	glfwDestroyWindow(window);

    //nk_glfw3_shutdown(&glfw);

	// Terminate GLFW before ending the program
	glfwTerminate();

}

void Window_Run() {

    AssetPool_Init();
    if (Window_Init()) {return;}
    Window_Loop();
    Window_Exit();
    AssetPool_Free();

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

        overlap = max(0, min(wx + ww, mx + mw) - max(wx, mx)) * 
                  max(0, min(wy + wh, my + mh) - max(wy, my));

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
    float pixel[3];

    // Bind the framebuffer and read the corresponding pixel.
    FrameBuffer_Bind(&entityTexture);
    glReadPixels(x, windowSize[1]-y-1, 1, 1, GL_RGB, GL_FLOAT, pixel);
    FrameBuffer_Unbind(&entityTexture);

    // Return the value of this pixel.
    return (int)pixel[0] - 1;

}
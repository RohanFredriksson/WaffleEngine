#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "debugdraw.h"
#include "wmath.h"
#include "window.h"
#include "camera.h"
#include "shader.h"
#include "shaderpool.h"

#define MAX_LINES 5000
#define VERTEX_ARRAY_LENGTH (MAX_LINES * 6 * 2)
#define CIRCLE_POINTS 20

struct DebugLine {
    vec2 from;
    vec2 to;
    vec3 colour;
    unsigned int lifetime;
};
typedef struct DebugLine DebugLine;

DebugLine* DebugDraw_Lines;
unsigned int DebugDraw_NumLines;
float* DebugDraw_Vertices;
Shader* DebugDraw_Shader;
unsigned int DebugDraw_VAO;
unsigned int DebugDraw_VBO;
bool DebugDraw_Started;

void DebugDraw_Init() {

    DebugDraw_Lines = (DebugLine*) malloc(MAX_LINES * sizeof(DebugLine));
    DebugDraw_NumLines = 0;
    DebugDraw_Vertices = (float*) malloc(VERTEX_ARRAY_LENGTH * sizeof(float));
    DebugDraw_Shader = ShaderPool_Get("assets/shaders/debug.vert", "assets/shaders/debug.frag");
    DebugDraw_Started = 0;

}

void DebugDraw_Start() {

    // Generate and bind a Vertex Array Object
    glGenVertexArrays(1, &DebugDraw_VAO);
    glBindVertexArray(DebugDraw_VAO);

    // Allocate space for vertices
    glGenBuffers(1, &DebugDraw_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, DebugDraw_VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_LINES * VERTEX_ARRAY_LENGTH * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    // Enable the vertex array attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glLineWidth(2.0f);
    DebugDraw_Started = 1;

}

void DebugDraw_BeginFrame() {

    if (!DebugDraw_Started) {
        DebugDraw_Start();
    }

    for (int i = 0; i < DebugDraw_NumLines; i++) {
        DebugLine* current = DebugDraw_Lines + i;
        current->lifetime--;
        if (current->lifetime < 0) {
            memmove(current, current+1, (DebugDraw_NumLines - i) * sizeof(DebugLine));
            DebugDraw_NumLines--;
            i--;
        }
    }

}

void DebugDraw_Draw() {

    if (DebugDraw_NumLines <= 0) {
        return;
    }

    int index = 0;
    for (int i = 0; i < DebugDraw_NumLines; i++) {
        DebugLine* current = DebugDraw_Lines + i;
        for (int j = 0; j < 2; j++) {
            
            vec2 position;
            if (j == 0) {glm_vec2_copy(current->from, position);}
            else {glm_vec2_copy(current->to, position);}

            vec3 colour;
            glm_vec3_copy(current->colour, colour);

            // Load positions
            DebugDraw_Vertices[index + 0] = position[0];
            DebugDraw_Vertices[index + 1] = position[1];
            DebugDraw_Vertices[index + 2] = -10.0f;

            // Load colour
            DebugDraw_Vertices[index + 3] = colour[0];
            DebugDraw_Vertices[index + 4] = colour[1];
            DebugDraw_Vertices[index + 5] = colour[2];
            index = index + 6;

        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, DebugDraw_VBO);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_ARRAY_LENGTH * sizeof(float), DebugDraw_Vertices, GL_DYNAMIC_DRAW);

    Shader_Use(DebugDraw_Shader);
    Shader_UploadMat4(DebugDraw_Shader, "uProjection", Window_GetScene()->camera.projection);
    Shader_UploadMat4(DebugDraw_Shader, "uView", Window_GetScene()->camera.view);

    glBindVertexArray(DebugDraw_VAO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_LINES, 0, DebugDraw_NumLines);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);

    Shader_Detach(DebugDraw_Shader);

}

void DebugDraw_Free() {
    free(DebugDraw_Lines);
    free(DebugDraw_Vertices);
}

void DebugDraw_AddLine2D(vec2 from, vec2 to, vec3 colour, int lifetime) {

    DebugLine l;
    glm_vec2_copy(from, l.from);
    glm_vec2_copy(to, l.to);
    glm_vec3_copy(colour, l.colour);
    l.lifetime = lifetime;

    if (DebugDraw_NumLines >= MAX_LINES) {
        return;
    }

    DebugDraw_Lines[DebugDraw_NumLines] = l;
    DebugDraw_NumLines++;

}

void DebugDraw_AddBox2D(vec2 centre, vec2 dimensions, float rotation, vec3 colour, int lifetime) {

    vec2 min;
    glm_vec2_scale(dimensions, 0.5f, min);
    glm_vec2_sub(centre, min, min);

    vec2 max;
    glm_vec2_scale(dimensions, 0.5f, max);
    glm_vec2_add(centre, max, max);

    vec2 vertices[4];
    vertices[0][0] = min[0];
    vertices[0][1] = min[1];
    vertices[1][0] = min[0];
    vertices[1][1] = max[1];
    vertices[2][0] = max[0];
    vertices[2][1] = max[1];
    vertices[3][0] = max[0];
    vertices[3][1] = min[1];

    if (rotation != 0.0f) {
        for (int i = 0; i < 4; i++) {
            WMath_Rotate(vertices[i], rotation, centre);
        }
    }

    DebugDraw_AddLine2D(vertices[0], vertices[1], colour, lifetime);
    DebugDraw_AddLine2D(vertices[0], vertices[3], colour, lifetime);
    DebugDraw_AddLine2D(vertices[1], vertices[2], colour, lifetime);
    DebugDraw_AddLine2D(vertices[2], vertices[3], colour, lifetime);

}

void DebugDraw_AddCircle(vec2 centre, float radius, vec3 colour, int lifetime) {

    vec2 points[CIRCLE_POINTS];
    int increment = 360 / CIRCLE_POINTS;
    int currentAngle = 0;

    for (int i = 0; i < CIRCLE_POINTS; i++) {
        
        vec2 tmp = { 0, radius };
        WMath_Rotate(tmp, currentAngle, (vec2) { 0, 0 });
        glm_vec2_add(tmp, centre, points[i]);

        if (i > 0) {DebugDraw_AddLine2D(points[i - 1], points[i], colour, lifetime);}
        currentAngle = currentAngle + increment;

    }
    DebugDraw_AddLine2D(points[CIRCLE_POINTS - 1], points[0], colour, lifetime);

}
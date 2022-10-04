#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "assetpool.h"
#include "renderer.h"
#include "external.h"
#include "texture.h"
#include "spriterenderer.h"
#include "shader.h"
#include "window.h"
#include "camera.h"

#define POS_SIZE 2
#define COLOUR_SIZE 4
#define TEX_COORDS_SIZE 2
#define TEX_ID_SIZE 1

#define POS_OFFSET 0
#define COLOUR_OFFSET (POS_OFFSET + POS_SIZE * sizeof(float))
#define TEX_COORDS_OFFSET (COLOUR_OFFSET + COLOUR_SIZE * sizeof(float))
#define TEX_ID_OFFSET (TEX_COORDS_OFFSET + TEX_COORDS_SIZE * sizeof(float))

#define VERTEX_SIZE 9
#define VERTEX_SIZE_BYTES (VERTEX_SIZE * sizeof(float))

#define TEXTURES_SIZE 7
#define INITIAL_BATCHES_SIZE 8
#define MAX_BATCH_SIZE 1000

Shader* Renderer_CurrentShader = NULL;

void Renderer_Init(Renderer* r) {
    r->batches = malloc(INITIAL_BATCHES_SIZE * sizeof(struct RenderBatch));
    r->numBatches = 0;
    r->sizeBatches = INITIAL_BATCHES_SIZE;
}

void Renderer_AddGameObject(Renderer* r, GameObject* go) {

    for (int i = 0; i < go->numComponents; i++) {
        if (strcmp(go->components[i]->type, "SpriteRenderer") == 0) {
            Renderer_AddSprite(r, (SpriteRenderer*) go->components[i]->data);
        }
    }

}

void Renderer_AddSprite(Renderer* r, SpriteRenderer* s) {

    bool added = 0;
    for (int i = 0; i < r->numBatches; i++) {
        RenderBatch* currentBatch = r->batches + i;
        if (RenderBatch_HasRoom(currentBatch) && RenderBatch_HasTextureRoom(currentBatch) && currentBatch->zIndex == s->zIndex) {
            if (s->sprite->texture != NULL || (RenderBatch_HasTexture(currentBatch, s->sprite->texture) || RenderBatch_HasTextureRoom(currentBatch))) {
                RenderBatch_AddSprite(currentBatch, s);
                added = 1;
                break;
            }
        }
    }

    if (!added) {

        // If there is no more space, allocate some more.
        if (r->numBatches == r->sizeBatches) {
            r->batches = realloc(r->batches, r->sizeBatches * 2 * sizeof(RenderBatch));
            r->sizeBatches = r->sizeBatches * 2;
        }

        // Initialise the new batch.

        // If there are no renderbatches, add a new one to the front of the array.
        if (r->numBatches == 0) {
            RenderBatch_Init(r->batches, r, s->zIndex);
            RenderBatch_AddSprite(r->batches, s);
            r->numBatches++;
        }

        // If the new renderbatch has a z index less than the first one, then add it to the
        // front of the array.
        else if (r->batches[0].zIndex >= s->zIndex) {
            memmove(r->batches + 1, r->batches, r->numBatches * sizeof(RenderBatch));
            RenderBatch_Init(r->batches, r, s->zIndex);
            RenderBatch_AddSprite(r->batches, s);
            r->numBatches++;
        }

        // If the new renderbatch has a z index greater than the last one, append to the end.
        else if (r->batches[r->numBatches-1].zIndex <= s->zIndex) {
            RenderBatch_Init(r->batches + r->numBatches, r, s->zIndex);
            RenderBatch_AddSprite(r->batches + r->numBatches, s);
            r->numBatches++;
        }  

        // Maintain the sortedness.
        else {
            for (int i = 1; i < r->numBatches; i++) {
                RenderBatch* previousBatch = r->batches + i - 1;
                RenderBatch* currentBatch = r->batches + i;
                if (previousBatch->zIndex < s->zIndex && currentBatch->zIndex >= s->zIndex) {
                    memmove(r->batches + i + 1, r->batches + i, (r->numBatches - i) * sizeof(RenderBatch));
                    RenderBatch_Init(r->batches + i, r, s->zIndex);
                    RenderBatch_AddSprite(r->batches + i, s);
                    r->numBatches++;
                    break;
                }
            }
        }
    }

}

void Renderer_RemoveGameObject(Renderer* r, GameObject* go) {
    for (int i = 0; i < r->numBatches; i++) {
        RenderBatch_RemoveGameObject(r->batches + i, go);
    }
}

void Renderer_RemoveSprite(Renderer* r, SpriteRenderer* s) {
    for (int i = 0; i < r->numBatches; i++) {
        RenderBatch_RemoveSprite(r->batches + i, s);
    }
}

void Renderer_BindShader(Shader* s) {
    Renderer_CurrentShader = s;
}

Shader* Renderer_GetBoundShader() {
    return Renderer_CurrentShader;
}

void Renderer_Render(Renderer* r) {
    Shader_Use(Renderer_CurrentShader);
    for (int i = 0; i < r->numBatches; i++) {
        RenderBatch_Render(r->batches + i);
    }
}

void Renderer_Free(Renderer* r) {
    for (int i = 0; i < r->numBatches; i++) {
        RenderBatch_Free(r->batches + i);
    }
    free(r->batches);
}

void RenderBatch_Init(RenderBatch* r, Renderer* renderer, int zIndex) {

    r->renderer = renderer;
    r->sprites = (SpriteRenderer**) malloc(MAX_BATCH_SIZE * sizeof(SpriteRenderer*));
    r->numSprites = 0;
    r->sizeSprites = MAX_BATCH_SIZE;
    r->textures = (Texture**) malloc(TEXTURES_SIZE * sizeof(Texture**));
    r->numTextures = 0;
    r->hasRoom = 1;
    r->hasTextureRoom = 1;
    r->vertices = (float*) malloc(MAX_BATCH_SIZE * 4 * VERTEX_SIZE * sizeof(float));
    r->zIndex = zIndex;

    // Generate and bind a Vertex Array Object
    glGenVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);

    // Allocate space for vertices
    glGenBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_BATCH_SIZE * 4 * VERTEX_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    // Create and upload the indices buffer.
    int ebo;
    glGenBuffers(1, &ebo);
    int* indices = (int*) malloc(MAX_BATCH_SIZE * 6 * sizeof(int));
    RenderBatch_GenerateIndices(indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_BATCH_SIZE * 6 * sizeof(int), indices, GL_STATIC_DRAW);
    free(indices);

    // Enable the buffer attribute pointers
    glVertexAttribPointer(0, POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*) POS_OFFSET);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, COLOUR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*) COLOUR_OFFSET);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, TEX_COORDS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*) TEX_COORDS_OFFSET);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, TEX_ID_SIZE, GL_FLOAT, GL_FALSE, VERTEX_SIZE_BYTES, (void*) TEX_ID_OFFSET);
    glEnableVertexAttribArray(3);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void RenderBatch_Render(RenderBatch* r) {

    bool rebufferData = 0;
    for (int i = 0; i < r->numSprites; i++) {

        if(r->sprites[i]->isDirty) {
            
            if (!RenderBatch_HasTexture(r, r->sprites[i]->sprite->texture) && r->sprites[i]->sprite->texture != NULL) {
                Renderer_RemoveSprite(r->renderer, r->sprites[i]);
                Renderer_AddSprite(r->renderer, r->sprites[i]);
                i--;
            } 
            
            else {
                RenderBatch_LoadVertexProperties(r, i);
                r->sprites[i]->isDirty = 0;
                rebufferData = 1;
            }

        }

        if (r->sprites[i]->zIndex != r->zIndex) {
            RenderBatch_RemoveSprite(r, r->sprites[i]);
            Renderer_AddSprite(r->renderer, r->sprites[i]);
            i--;
        }

    }

    if (rebufferData) {
        glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_BATCH_SIZE * 4 * VERTEX_SIZE * sizeof(float), r->vertices);
    }

    // Use shader
    Shader* shader = Renderer_GetBoundShader(r->renderer);
    Shader_Use(shader);

    mat4 view;
    mat4 projection;
    Camera_GetProjection(&(Window_GetScene()->camera), projection);
    Camera_GetView(&(Window_GetScene()->camera), view);
    Shader_UploadMat4(shader, "uProjection", projection);
    Shader_UploadMat4(shader, "uView", view);

    for (int i = 0; i < r->numTextures; i++) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        Texture_Bind(r->textures[i]);
    }
    int slots[] = {0, 1, 2, 3, 4, 5, 6, 7};
    Shader_UploadIntArray(shader, "uTextures", TEXTURES_SIZE+1, slots);

    glBindVertexArray(r->vao);
    glDrawElements(GL_TRIANGLES, r->numSprites * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (int i = 0; i < r->numTextures; i++) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        Texture_Unbind(r->textures[i]);
    }
    Shader_Detach(shader);

}

void RenderBatch_AddGameObject(RenderBatch* r, GameObject* go) {

    for (int i = 0; i < go->numComponents; i++) {
        if (strcmp(go->components[i]->type, "SpriteRenderer")) {
            RenderBatch_AddSprite(r, (SpriteRenderer*) go->components[i]->data);
        }
    }

}

void RenderBatch_AddSprite(RenderBatch* r, SpriteRenderer* s) {

    // If full do not attempt to add.
    if (!r->hasRoom) {
        return;
    }

    // Get index and add the render object
    int index = r->numSprites;
    r->sprites[index] = s;
    r->numSprites = r->numSprites + 1;

    // Add the sprites texture, if the batch does not have it.
    if (s->sprite->texture != NULL) {
        if (!RenderBatch_HasTexture(r, s->sprite->texture)) {
            RenderBatch_AddTexture(r, s->sprite->texture);
        }
    }

    // Add properites to local vertices array.
    RenderBatch_LoadVertexProperties(r, index);

    if (r->numSprites >= MAX_BATCH_SIZE) {
        r->hasRoom = 0;
    }

}

void RenderBatch_RemoveGameObject(RenderBatch* r, GameObject* go) {

    for (int i = 0; i < go->numComponents; i++) {
        if (strcmp(go->components[i]->type, "SpriteRenderer") == 0) {
            RenderBatch_RemoveSprite(r, (SpriteRenderer*) go->components[i]->data);
        }
    }

}

void RenderBatch_RemoveSprite(RenderBatch* r, SpriteRenderer* s) {

    for (int i = 0; i < r->numSprites; i++) {
        if (r->sprites[i] == s) {
            memmove(r->sprites + i, r->sprites + i + 1, (r->numSprites - i - 1) * sizeof(SpriteRenderer*));
            r->numSprites--;
        }
    }

}

void RenderBatch_LoadVertexProperties(RenderBatch* r, int index) {
    
    SpriteRenderer* sprite = r->sprites[index];

    // Find the offset with array (4 vertices per sprite).
    int offset = index * 4 * VERTEX_SIZE;

    vec4 colour;
    glm_vec4_copy(sprite->colour, colour);
    vec2 texCoords[4];
    glm_vec4_copy(sprite->sprite->texCoords[0], texCoords[0]);
    glm_vec4_copy(sprite->sprite->texCoords[1], texCoords[1]);
    glm_vec4_copy(sprite->sprite->texCoords[2], texCoords[2]);
    glm_vec4_copy(sprite->sprite->texCoords[3], texCoords[3]);

    int texId = 0;
    if (sprite->sprite->texture != NULL) {
        for (int i = 0; i < r->numTextures; i++) {
            if (r->textures[i] == sprite->sprite->texture) {
                texId = i + 1;
                break;
            }
        }
    }

    bool isRotated = sprite->transform->rotation != 0.0f;
    mat4 transformMatrix;
    glm_mat4_identity(transformMatrix);

    if (isRotated) {
        vec3 translateVector = {sprite->transform->pos[0], sprite->transform->pos[1], 0.0f};
        glm_translate(transformMatrix, translateVector);
        vec3 rotateVector = {0.0f, 0.0f, 1.0f};
        glm_rotate(transformMatrix, sprite->transform->rotation / (float)(180.0 / M_PI), rotateVector);
        vec3 scaleVector = {sprite->transform->size[0], sprite->transform->size[1], 1.0f};
        glm_scale(transformMatrix, scaleVector);
    }

    // Add vertices with the appropriate properties.
    float xAdd = 0.5f;
    float yAdd = 0.5f;

    for (int i = 0; i < 4; i++) {

        if (i == 1) {yAdd = -0.5f;}
        else if (i == 2) {xAdd = -0.5f;}
        else if (i == 3) {yAdd = 0.5f;}

        vec4 currentPos = {
            sprite->transform->pos[0] + (xAdd * sprite->transform->size[0]),
            sprite->transform->pos[1] + (yAdd * sprite->transform->size[1]),
            0.0f, 1.0f
        };

        if (isRotated) {
            vec4 posVector = {xAdd, yAdd, 0.0f, 1.0f};
            glm_mat4_mulv(transformMatrix, posVector, currentPos);
        }

        // Load Position
        r->vertices[offset] = currentPos[0];
        r->vertices[offset + 1] = currentPos[1];

        // Load Colour
        r->vertices[offset + 2] = colour[0];
        r->vertices[offset + 3] = colour[1];
        r->vertices[offset + 4] = colour[2];
        r->vertices[offset + 5] = colour[3];

        // Load Texture Coordinates
        r->vertices[offset + 6] = texCoords[i][0];
        r->vertices[offset + 7] = texCoords[i][1];

        // Load Texture ID
        r->vertices[offset + 8] = texId;

        offset += VERTEX_SIZE;

    }

}

void RenderBatch_GenerateIndices(int* elements) {
    for (int i = 0; i < MAX_BATCH_SIZE; i++) {
        RenderBatch_LoadElementIndices(elements, i);
    }
}

void RenderBatch_LoadElementIndices(int* elements, int index) {

    int offsetArrayIndex = 6 * index;
    int offset = 4 * index;

    // Triangle 1
    elements[offsetArrayIndex] = offset + 3;
    elements[offsetArrayIndex+1] = offset + 2;
    elements[offsetArrayIndex+2] = offset + 0;

    // Triangle 2
    elements[offsetArrayIndex+3] = offset + 0;
    elements[offsetArrayIndex+4] = offset + 2;
    elements[offsetArrayIndex+5] = offset + 1;

}

bool RenderBatch_HasRoom(RenderBatch* r) {
    return r->hasRoom;
}

bool RenderBatch_HasTextureRoom(RenderBatch* r) { 
    return r->hasTextureRoom;
}

bool RenderBatch_HasTexture(RenderBatch* r, Texture* t) {
    
    // Check if the given texture pointer exists in the array.
    for (int i = 0; i < r->numTextures; i++) {
        Texture* current = r->textures[i];
        if (current == t) {
            return 1;
        }
    }

    return 0;
}

void RenderBatch_AddTexture(RenderBatch* r, Texture* t) {

    if (!RenderBatch_HasTextureRoom(r)) {
        return;
    }

    // Add the texture.
    r->textures[r->numTextures] = t;
    r->numTextures++;

    if (r->numTextures == TEXTURES_SIZE) {
        r->hasTextureRoom = 0;
    }

}

int RenderBatch_Compare(const void* a, const void* b) {
    int aIndex = (*(RenderBatch**) a)->zIndex;
    int bIndex = (*(RenderBatch**) b)->zIndex;
    return aIndex - bIndex;
}

void RenderBatch_Free(RenderBatch* r) {
    free(r->sprites);
    free(r->textures);
    free(r->vertices);
}
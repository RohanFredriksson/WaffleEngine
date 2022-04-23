#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "external.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture_Init(Texture* t, const char* filename) {
    
    // Allocate space to store the string.
    t->filename = (char*) malloc(strlen(filename)+1);
    memcpy(t->filename, filename, strlen(filename)+1);

    // Generate texture on GPU
    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the image
    stbi_set_flip_vertically_on_load(1);
    unsigned char* image = stbi_load(filename, &t->width, &t->height, &t->channels, 0);

    // Image loaded successfully.
    if (image != NULL) {

        // Upload image to gpu.
        if (t->channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        }

        else if (t->channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        }

        else {
            printf("ERROR::TEXTURE::INVALID_IMAGE_CHANNELS '%d'\n", t->channels);
            exit(0);
        }

    }

    // Image did not load.
    else {
        printf("ERROR::TEXTURE::IMAGE_LOAD_FAILED '%s'\n", filename);
        exit(0);
    }

    // Free the image
    stbi_image_free(image);

}

void Texture_Bind(Texture* t) {
    glBindTexture(GL_TEXTURE_2D, t->id);
}

void Texture_Unbind(Texture* t) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture_Free(Texture* t) {
    free(t->filename);
}
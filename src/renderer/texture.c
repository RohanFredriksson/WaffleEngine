#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "external.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

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

void Texture_New(Texture* t, int width, int height) {

    // Store the width/height values.
    t->width = width;
    t->height = height;

    // Allocate space to store the string.
    char filename[] = "Generated";
    t->filename = (char*) malloc(strlen(filename)+1);
    memcpy(t->filename, filename, strlen(filename)+1);

    // Generate texture on GPU
    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

}

void Texture_New_RGB32F(Texture* t, int width, int height) {

    // Store the width/height values.
    t->width = width;
    t->height = height;

    // Allocate space to store the string.
    char filename[] = "Generated";
    t->filename = (char*) malloc(strlen(filename)+1);
    memcpy(t->filename, filename, strlen(filename)+1);

    // Generate texture on GPU
    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, t->width, t->height, 0, GL_RGB, GL_FLOAT, 0);

}

void Texture_Save(Texture* t, const char* filename) {
    stbi_flip_vertically_on_write(1);
    unsigned char* image = malloc(t->width * t->height * 4 * sizeof(unsigned char));
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    stbi_write_png(filename, t->width, t->height, 4, image, t->width * 4 * sizeof(unsigned char));
    free(image);
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
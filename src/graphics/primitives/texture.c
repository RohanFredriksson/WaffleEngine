#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "external.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

bool Texture_Init(Texture* t, char* filename) {
    
    // Store the string.
    t->filename = filename;

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
            return 0;
        }

    }

    // Image did not load.
    else {
        printf("ERROR::TEXTURE::IMAGE_LOAD_FAILED '%s'\n", filename);
        return 0;
    }

    // Free the image
    stbi_image_free(image);
    return 1;

}

void Texture_New(Texture* t, int width, int height) {

    // Store the width/height values.
    t->width = width;
    t->height = height;

    // Store the string.
    t->filename = "Generated";

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

    // Store the string.
    t->filename = "Generated";

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

void Texture_Init_Missing(Texture* t) {

    t->filename = "missing";

    // Generate texture on GPU
    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the image
    unsigned char image[] = {0,0,0,255,0,255,0,0,255,0,255,0,0};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

}

void Texture_Init_RGBA(Texture* t, unsigned char* image, int width, int height) {

    // Store the string.
    t->filename = "Generated";

    // Generate texture on GPU
    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

}

/*
void Texture_Save(Texture* t, char* filename) {
    stbi_flip_vertically_on_write(1);
    unsigned char* image = malloc(t->width * t->height * 4 * sizeof(unsigned char));
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    stbi_write_png(filename, t->width, t->height, 4, image, t->width * 4 * sizeof(unsigned char));
    free(image);
}
*/

void Texture_Bind(Texture* t) {
    glBindTexture(GL_TEXTURE_2D, t->id);
}

void Texture_Unbind(Texture* t) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture_Free(Texture* t) {
    glDeleteTextures(1, &t->id);
}
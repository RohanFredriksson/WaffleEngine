#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "external.h"
#include "texture.h"

void Texture_Init(Texture* t, const char* filename) {
    
    // Allocate space to store the string.
    t->filename = (char*) malloc(strlen(filename)+1);
    memcpy(t->filename, filename, strlen(filename)+1);

    // Generate texture on GPU
    t->id = glGenTextures();
    glBindTexture(GL_TEXTURE_2D, t->id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load the image
    unsigned char* image = stbi_load(filename, &t->width, &t->height, &t->channels, 0);

    if (image != NULL) {
        if (t->channels == 3) {glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);}
        else if (t->channels == 4) {glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);}
        else {printf("Error: (Texture) Unknown number of channels '%d'\n", t->channels);}
    }

    else {
        printf("Error: (Texture) Could not load image '%s'\n", filename);
    }

    // Free the image
    stbi_free(image);

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
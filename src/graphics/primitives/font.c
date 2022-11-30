#include "font.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define FONT_NUM_CHARACTERS 128

bool Font_Init(Font* font, char* filename, int size) {

    font->filename = filename;
    font->size = size;

    // Read the contents of the file.
    long fileSize;
    unsigned char* fontBuffer;
    FILE* fontFile = fopen(filename, "rb");
    if (fontFile == NULL) {
        printf("ERROR::FONT::INIT::FILE_NOT_FOUND\n");
        return 0;
    }
    fseek(fontFile, 0, SEEK_END);
    fileSize = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    fontBuffer = malloc(fileSize);
    fread(fontBuffer, fileSize, 1, fontFile);
    fclose(fontFile);

    // Prepare the font
    if (!stbtt_InitFont(&font->info, fontBuffer, 0)) {
        printf("ERROR::FONT::INIT::FONT_LOADING_FAILED\n");
        return 0;
    }

    // Store all sprites in an array.
    font->sprites = malloc(FONT_NUM_CHARACTERS * sizeof(Sprite));

    // Find the scale for a certain pixel height.
    float scale = stbtt_ScaleForPixelHeight(&font->info, size);
    
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font->info, &ascent, &descent, &lineGap);
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    // Determine the total width required to display all characters.
    int width = 0;
    int height;
    for (int i = 0; i < FONT_NUM_CHARACTERS; i++) {
        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&font->info, i, &ax, &lsb);
        width += roundf(ax * scale);
    }

    // Determine the most optimal square shape for the texture.
    int area = width * size;
    width = sqrtf(area) + 1;
    height = ((width / size) + 2) * size;
    
    // Allocate memory to store the grayscale image.
    unsigned char* mask = calloc(width * height, sizeof(unsigned char));

    // Render each character onto the grayscale image.
    int x = 0;
    int line = 0;
    for (int i = 0; i < FONT_NUM_CHARACTERS; i++) {

        char character = (char) i;
        char name[] = { character, '\0' };

        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&font->info, i, &ax, &lsb);

        // If the character is going out of the buffer, move to new line
        int advance = roundf(ax * scale);
        if (x + advance >= width) {
            x = 0;
            line++;
        }

        // Get bounding box for characters.
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&font->info, i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        // Compute the y value
        int y = (line * size) + ascent + c_y1;
        
        // Create the sprite object.
        float x0 = (float) (x) / (float) width;
        float x1 = (float) (x + advance) / (float) width;
        float y0 = (float) (y + size) / (float) height;
        float y1 = (float) (y) / (float) height;
        
        vec2 texCoords[4];
        texCoords[0][0] = x1;
        texCoords[0][1] = y1;
        texCoords[1][0] = x1;
        texCoords[1][1] = y0;
        texCoords[2][0] = x0;
        texCoords[2][1] = y0;
        texCoords[3][0] = x0;
        texCoords[3][1] = y1;

        // Initialise the sprite object.
        Sprite* s = font->sprites + i;
        Sprite_SetName(s, name);
        Sprite_SetTexCoords(s, texCoords);

        // Render the character
        int byteOffset = x + roundf(lsb * scale) + (y * width);
        stbtt_MakeCodepointBitmap(&font->info, mask + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, i);

        // Advance x
        x += advance;

    }

    // Convert the grayscale to a transparent image.
    unsigned char* image = calloc(width * height * 4, sizeof(unsigned char));
    for (int i = 0; i < width * height; i++) {
        
        if (mask[i] > 0) {
            image[4 * i + 0] = 255;
            image[4 * i + 1] = 255;
            image[4 * i + 2] = 255;
        } 
        
        else {
            image[4 * i + 0] = 0;
            image[4 * i + 1] = 0;
            image[4 * i + 2] = 0;
        }
        
        image[4 * i + 3] = mask[i];
    }

    // Create the texture
    Texture_Init_RGBA(&font->texture, image, width, height);

    // Add the texture to each sprite.
    for (int i = 0; i < FONT_NUM_CHARACTERS; i++) {
        Sprite_SetTexture(font->sprites + i, &font->texture);
    }

    // Debug output
    // stbi_write_png("out.png", width, height, 4, image, width * 4);

    free(fontBuffer);
    free(mask);
    free(image);

    return 1;
}

void Font_Free(Font* font) {
    free(font->sprites);
    Texture_Free(&font->texture);
}

Sprite* Font_Get(Font* font, char code) {
    int index = (int) code;
    if (index < 0 || index >= FONT_NUM_CHARACTERS) {return NULL;}
    return font->sprites + index;
}

float Font_GetScaleForHeight(Font* font, float height) {
    return stbtt_ScaleForPixelHeight(&font->info, height);
}

int Font_Advance(Font* font, char code) {
    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(&font->info, code, &ax, &lsb);
    return ax;
}

int Font_Kerning(Font* font, char current, char next) {
    return stbtt_GetCodepointKernAdvance(&font->info, current, next);
}

Texture* Font_GetTexture(Font* font) {
    return &font->texture;   
}

bool Font_Equals(Font* a, Font* b) {
    return ((strcmp(a->filename, b->filename) == 0) && (a->size == b->size));
}
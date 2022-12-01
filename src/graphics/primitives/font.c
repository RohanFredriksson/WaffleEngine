#include "font.h"
#include "assetpool.h"
#include "wio.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define FONT_NUM_CHARACTERS 128

bool Font_Init(Font* font, char* filename, float size) {

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
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0)) {
        printf("ERROR::FONT::INIT::FONT_LOADING_FAILED\n");
        return 0;
    }

    // Create arrays to store info.
    font->sprites = malloc(FONT_NUM_CHARACTERS * sizeof(Sprite));
    font->advances = malloc(FONT_NUM_CHARACTERS * sizeof(int));

    // Find the scale for a certain pixel height.
    float scale = stbtt_ScaleForPixelHeight(&info, size);

    // Get font's vertical metrics
    stbtt_GetFontVMetrics(&info, &font->ascent, &font->descent, &font->lineGap);
    int ascent = roundf(scale * font->ascent);

    // Determine the total width required to display all characters.
    int width = 0;
    int height;
    for (int i = 0; i < FONT_NUM_CHARACTERS; i++) {
        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&info, i, &ax, &lsb);
        width += roundf(ax * scale);
    }

    int average = width / FONT_NUM_CHARACTERS;
    int hSpacing = roundf(0.125f * average);

    // Create a square shaped image for the texture.
    int area = width * size * 1.25f;
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
        stbtt_GetCodepointHMetrics(&info, i, &ax, &lsb);
        font->advances[i] = ax;

        // If the character is going out of the buffer, move to new line
        int advance = roundf(ax * scale);
        if (x + advance + hSpacing >= width) {
            x = 0;
            line++;
        }

        // Get bounding box for characters.
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        // Compute the y value
        int y = (line * size) + ascent + c_y1;
        
        // Create the sprite object.
        float x0 = (float) (x) / (float) width;
        float x1 = (float) (x + advance) / (float) width;
        float y0 = (float) (y - ascent - c_y1 + size) / (float) height;
        float y1 = (float) (y - ascent - c_y1) / (float) height;
        
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
        stbtt_MakeCodepointBitmap(&info, mask + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, i);

        // Advance x
        x += advance + hSpacing;

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
    stbi_write_png("out.png", width, height, 4, image, width * 4);

    free(fontBuffer);
    free(mask);
    free(image);

    return 1;
}

cJSON* Font_Serialise(Font* font) {
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "filename", font->filename);
    WIO_AddFloat(json, "size", font->size);
    return json;
}

Font* Font_Parse(cJSON* json) {

    char* filename;
    float size;

    if (!WIO_ParseString(json, "filename", &filename)) {return NULL;}
    if (!WIO_ParseFloat(json, "size", &size)) {return NULL;}

    Font* font = FontPool_Get(filename, size);
    return font;

}

void Font_Load(cJSON* json) {
    Font_Parse(json);
}

void Font_Free(Font* font) {
    free(font->sprites);
    free(font->advances);
    Texture_Free(&font->texture);
}

Sprite* Font_Get(Font* font, char code) {
    int index = (int) code;
    if (index < 0 || index >= FONT_NUM_CHARACTERS) {return NULL;}
    return font->sprites + index;
}

float Font_GetScaleForHeight(Font* font, float height) {
    return height / (font->ascent - font->descent);
}

int Font_Advance(Font* font, char code) {
    int index = (int) code;
    return font->advances[index];
}

int Font_Ascent(Font* font) {
    return font->ascent;
}

int Font_Descent(Font* font) {
    return font->descent;
}

int Font_LineGap(Font* font) {
    return font->lineGap;
}

Texture* Font_GetTexture(Font* font) {
    return &font->texture;   
}

bool Font_Equals(Font* a, Font* b) {
    return ((strcmp(a->filename, b->filename) == 0) && (a->size == b->size));
}
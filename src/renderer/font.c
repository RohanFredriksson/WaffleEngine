#include "font.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

void Font_Init(Font* font, char* filepath, int fontSize) {

    long size;
    unsigned char* fontBuffer;
    
    FILE* fontFile = fopen(filepath, "rb");
    if (fontFile == NULL) {
        printf("ERROR::FONT::INIT::FILE_NOT_FOUND\n");
        return;
    }
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    
    fontBuffer = malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    // Prepare the font
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0)) {
        printf("ERROR::FONT::INIT::FONT_LOADING_FAILED\n");
        return;
    }

    // Find the scale for a certain pixel height.
    float scale = stbtt_ScaleForPixelHeight(&info, fontSize);
    
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    // Determine the total width required to display all characters.
    int width = 0;
    int height;
    for (int i = 0; i < 128; i++) {
        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&info, i, &ax, &lsb);
        width += roundf(ax * scale);
    }

    int area = width * fontSize;
    width = sqrtf(area) + 1;
    height = ((width / fontSize) + 1) * fontSize;
    
    unsigned char* mask = calloc(width * height, sizeof(unsigned char));

    int x = 0;
    int line = 0;
    for (int i = 0; i < 128; i++) {

        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&info, i, &ax, &lsb);

        // If the character is going out of the buffer, move to new line
        int advance = roundf(ax * scale);
        if (x + advance > width) {
            x = 0;
            line++;
        }

        // Get bounding box for characters.
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

        // Compute the y value
        int y = (line * fontSize) + ascent + c_y1;
        
        // Render the character
        int byteOffset = x + roundf(lsb * scale) + (y * width);
        stbtt_MakeCodepointBitmap(&info, mask + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, i);

        // Advance x
        x += roundf(ax * scale);

        // USEFUL KERNING
        //int kern;
        //kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        //x += roundf(kern * scale);

    }

    // Convert the grayscale to a transparent image.
    unsigned char* image = calloc(width * height * 4, sizeof(unsigned char));
    for (int i = 0; i < width * height; i++) {
        image[4 * i + 0] = 255;
        image[4 * i + 1] = 255;
        image[4 * i + 2] = 255;
        image[4 * i + 3] = mask[i];
    }

    // Save output
    stbi_write_png("out.png", width, height, 4, image, width * 4);

    free(fontBuffer);
    free(mask);
    free(image);

}

void Font_Free(Font* font) {

}
#include "font.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

void Font_Init(Font* font, char* filepath, int fontSize) {

    long size;
    unsigned char* fontBuffer;
    
    FILE* fontFile = fopen(filepath, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    
    fontBuffer = malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    // Prepare the font
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0)) {
        printf("failed\n");
    }

    // Find the scale for a certain pixel height.
    float scale = stbtt_ScaleForPixelHeight(&info, fontSize);
    
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);

    char* word = "quick";

    // Determine the total width required to display all characters.
    int width = 0;
    int height;
    for (char i = 0; i < 5; i++) {
        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);
        width += roundf(ax * scale);
    }

    int area = width * fontSize;
    width = sqrtf(area) + 1;
    height = ((width / fontSize) + 1) * fontSize;
    
    unsigned char* bitmap = calloc(width * height, sizeof(unsigned char));

    int x = 0;
    int line = 0;
    for (char i = 0; i < 5; i++) {

        printf("LOOP START\n");

        int ax;
	    int lsb;
        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);

        // If the character is going out of the buffer, move to new line
        int advance = roundf(ax * scale);
        if (x + advance > width) {
            printf("NEW LINE\n");
            x = 0;
            line++;
        }

        // Get bounding box for characters.
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
    
        printf("%d %d %d %d %d\n", ax, c_x1, c_y1, c_x2, c_y2);

        // Compute the y value
        int y = (line * fontSize) + ascent + c_y1;
        
        // Render the character
        int byteOffset = x + roundf(lsb * scale) + (y * width);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, width, scale, scale, word[i]);

        // Advance x
        x += roundf(ax * scale);

        // USEFUL KERNING
        //int kern;
        //kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        //x += roundf(kern * scale);

    }
    
    printf("END\n");

    // Save output
    stbi_write_png("out.png", width, height, 1, bitmap, width);

    printf("SAVED\n");

    free(fontBuffer);
    free(bitmap);

}

/*
void Font_Free(Font* font) {

}

Sprite* Font_Get(Font* font, int code) {
    return NULL;
}

CharInfo* Font_Info(Font* font, int code) {
    return NULL;
}

Texture* Font_GetTexture(Font* font) {
    return NULL;
}
*/
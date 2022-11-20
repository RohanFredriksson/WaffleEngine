#include "spritesheet.h"

static char jsonext[] = ".json";

struct SpritesheetMetadata {
    bool valid;
    int width;
    int height;
    int num;
    char** names;
};
typedef struct SpritesheetMetadata SpritesheetMetadata;

static char* Spritesheet_GetMeta(const char* filename) {

    int i = 0;
    int lastDot = -1;
    while (filename[i] != '\0') {
        if (filename[i] == '.') {lastDot = i;}
        i++;
    }

    char* metaname = malloc((lastDot+strlen(jsonext) + 1) * sizeof(char));
    memcpy(metaname, filename, lastDot * sizeof(char));
    memcpy(metaname + lastDot, jsonext, strlen(jsonext) + 1);
    return metaname;

}

static char* Spritesheet_LoadSource(const char* filepath) {
    
    size_t size = 16;
    size_t length = 0;
    char* source = malloc(size);
    FILE* f = fopen(filepath, "r");
    
    char next;
    while (1) {
        
        next = fgetc(f);
        if (next == EOF) {
            break;
        }

        if (length >= size) {
            source = realloc(source, size * 2);
            size = size * 2;
        }

        source[length] = next;
        length++;

    }

    fclose(f);
    source = realloc(source, length + 1);
    source[length] = '\0';

    return source;   
}

static SpritesheetMetadata Spritesheet_ParseMeta(const char* metaname) {

    SpritesheetMetadata result = { 0, 0, 0, 0, NULL };
    char* contents = Spritesheet_LoadSource(metaname);
    
    cJSON* metadata = cJSON_Parse(contents);
    if (metadata == NULL) {
        printf("ERROR::SPRITESHEET::LOAD::JSON_PARSE_ERROR: \"%s\" could not be parsed.\n", metaname);
        cJSON_Delete(metadata);
        free(contents);
        return result;
    }

    cJSON* spriteWidth = cJSON_GetObjectItemCaseSensitive(metadata, "spriteWidth");
    if (spriteWidth == NULL || !cJSON_IsNumber(spriteWidth)) {
        printf("ERROR::SPRITESHEET::LOAD::JSON_PARSE_ERROR: \"%s\" does not contain \"spriteWidth\" attribute.\n", metaname);
        cJSON_Delete(metadata);
        free(contents);
        return result;
    }
    
    cJSON* spriteHeight = cJSON_GetObjectItemCaseSensitive(metadata, "spriteHeight");
    if (spriteHeight == NULL || !cJSON_IsNumber(spriteHeight)) {
        printf("ERROR::SPRITESHEET::LOAD::JSON_PARSE_ERROR: \"%s\" does not contain \"spriteHeight\" attribute.\n", metaname);
        cJSON_Delete(metadata);
        free(contents);
        return result;
    }

    int width = spriteWidth->valueint;
    int height = spriteHeight->valueint;

    if (width <= 0 || height <= 0) {
        printf("ERROR::SPRITESHEET::LOAD::JSON_PARSE_ERROR: \"%s\" has invalid sprite dimensions.\n", metaname);
        cJSON_Delete(metadata);
        free(contents);
        return result;
    }

    cJSON* spriteNames = cJSON_GetObjectItemCaseSensitive(metadata, "sprites");
    if (spriteNames == NULL) {
        printf("ERROR::SPRITESHEET::LOAD::JSON_PARSE_ERROR: \"%s\" does not contain \"sprites\" attribute.\n", metaname);
        cJSON_Delete(metadata);
        free(contents);
        return result;
    }

    int num = 0;
    int size = 16;
    char** names = malloc(size * sizeof(char*));

    int errorIndex = -1;
    cJSON* spriteName = NULL;
    cJSON_ArrayForEach(spriteName, spriteNames) {
    
        if (cJSON_IsString(spriteName) && errorIndex == -1) {
            
            char* name = malloc(strlen(spriteName->valuestring) + 1);
            memcpy(name, spriteName->valuestring, strlen(spriteName->valuestring) + 1);

            if (num >= size) {
                names = realloc(names, 2 * size * sizeof(char*));
                size = 2 * size;
            }

            names[num] = name;
            num++;

        } else {
            errorIndex = num;
        }

    }

    if (errorIndex != -1) {
        printf("ERROR::SPRITESHEET::LOAD::JSON_PARSE_ERROR: \"%s\": Index %d in \"sprites\" is not a string.\n", metaname, errorIndex);
        for (int i = 0; i < num; i++) {free(names[i]);}
        free(names);
        cJSON_Delete(metadata);
        free(contents);
        return result;
    }

    names = realloc(names, num * sizeof(char*));
    
    result.valid = 1;
    result.width = width;
    result.height = height;
    result.num = num;
    result.names = names;

    cJSON_Delete(metadata);
    free(contents);
    return result;

}

void Spritesheet_Load(const char* filename) {
    
    char* metaname = Spritesheet_GetMeta(filename);
    SpritesheetMetadata metadata = Spritesheet_ParseMeta(metaname);
    
    if (!metadata.valid) {
        free(metaname);
        return;
    }

    Texture* texture = TexturePool_Get(filename);
    int currentX = 0;
    int currentY = texture->height - metadata.height;

    for (int i = 0; i < metadata.num; i++) {

        float topY = (currentY + metadata.height) / (float) texture->height;
        float rightX = (currentX + metadata.width) / (float) texture->width;
        float leftX = currentX / (float) texture->width;
        float bottomY = currentY / (float) texture->height;

        vec2 texCoords[4];
        texCoords[0][0] = rightX;
        texCoords[0][1] = topY;
        texCoords[1][0] = rightX;
        texCoords[1][1] = bottomY;
        texCoords[2][0] = leftX;
        texCoords[2][1] = bottomY;
        texCoords[3][0] = leftX;
        texCoords[3][1] = topY;

        Sprite* sprite = malloc(sizeof(Sprite));
        Sprite_Init(sprite, texture);
        Sprite_SetTexCoords(sprite, texCoords);
        Sprite_SetSize(sprite, (vec2) { metadata.width, metadata.height });
        SpritePool_Put(metadata.names[i], sprite);

    }

    for (int i = 0; i < metadata.num; i++) {free(metadata.names[i]);}
    free(metadata.names);
    free(metaname);

}
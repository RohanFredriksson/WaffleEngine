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

static char* Spritesheet_GetMeta(char* filename) {

    int i = 0;
    int lastDot = -1;
    while (filename[i] != '\0') {
        if (filename[i] == '.') {lastDot = i;}
        i++;
    }

    char* metaname = malloc((lastDot+strlen(jsonext) + 1));
    memcpy(metaname, filename, lastDot);
    memcpy(metaname + lastDot, jsonext, strlen(jsonext) + 1);
    return metaname;

}

static SpritesheetMetadata Spritesheet_ParseMeta(char* metaname) {

    SpritesheetMetadata result = { 0, 0, 0, 0, NULL };
    char* contents = WIO_LoadSource(metaname);
    
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

void Spritesheet_Load(char* filename) {
    
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

        if (currentY < 0) {
            printf("ERROR::SPRITESHEET::LOAD::SPRITE_LOAD: \"%s\": Sprite \"%s\" could not be loaded.\n", metaname, metadata.names[i]);
            continue;
        }

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

        // If the sprite name is already taken, do not overwrite it.
        if (!SpritePool_Has(metadata.names[i])) {

            Sprite* sprite = malloc(sizeof(Sprite));
            Sprite_Init(sprite, texture, StringPool_Get(metadata.names[i]));
            Sprite_SetTexCoords(sprite, texCoords);
            Sprite_SetSize(sprite, (vec2) { metadata.width, metadata.height });
            SpritePool_Put(sprite);

        }

        currentX += metadata.width;
        if (currentX >= texture->width) {
            currentX = 0;
            currentY -= metadata.height;
        }

    }

    for (int i = 0; i < metadata.num; i++) {free(metadata.names[i]);}
    free(metadata.names);
    free(metaname);

}
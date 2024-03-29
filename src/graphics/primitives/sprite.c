#include <stdlib.h>
#include <string.h>
#include "sprite.h"
#include "texture.h"
#include "assetpool.h"

void Sprite_Init(Sprite* s, Texture* t, char* name) {
    
    s->texture = t;

    if (name != NULL) {s->name = StringPool_Get(name);} 
    else if (t != NULL) {s->name = StringPool_Get(t->filename);} 
    else {s->name = "empty";}

    s->texCoords[0][0] = 1;
    s->texCoords[0][1] = 1;
    s->texCoords[1][0] = 1;
    s->texCoords[1][1] = 0;
    s->texCoords[2][0] = 0;
    s->texCoords[2][1] = 0;
    s->texCoords[3][0] = 0;
    s->texCoords[3][1] = 1;

}

void Sprite_SetTexture(Sprite* s, Texture* texture) {
    s->texture = texture;
}

void Sprite_SetTexCoords(Sprite* s, vec2 texCoords[4]) {

    s->texCoords[0][0] = texCoords[0][0];
    s->texCoords[0][1] = texCoords[0][1];
    s->texCoords[1][0] = texCoords[1][0];
    s->texCoords[1][1] = texCoords[1][1];
    s->texCoords[2][0] = texCoords[2][0];
    s->texCoords[2][1] = texCoords[2][1];
    s->texCoords[3][0] = texCoords[3][0];
    s->texCoords[3][1] = texCoords[3][1];
    
}

void Sprite_SetName(Sprite* s, char* name) {
    s->name = StringPool_Get(name);
}

bool Sprite_Equals(Sprite* s1, Sprite* s2) {

    // If the textures are not the same, return 0.
    if (s1->texture != s2->texture) {
        return 0;
    }

    // If the texture coords are not the same, return 0.
    for (int i = 0; i < 4; i++) {
        if (s1->texCoords[i][0] != s2->texCoords[i][0]) {return 0;}
        if (s1->texCoords[i][1] != s2->texCoords[i][1]) {return 0;}
    }

    // If reached here, then the sprites are the same, return 1.
    return 1;

}

cJSON* Sprite_Serialise(Sprite* s) {

    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "name", s->name);
    cJSON* texture;
    if (s->texture != NULL) {texture = cJSON_CreateString(s->texture->filename);}
    else {texture = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "texture", texture);
    cJSON* texCoords = cJSON_CreateArray();
    for (int i = 0; i < 4; i++) {
        cJSON* texCoord = cJSON_CreateArray();
        cJSON* x = cJSON_CreateNumber((double) s->texCoords[i][0]);
        cJSON* y = cJSON_CreateNumber((double) s->texCoords[i][1]);
        cJSON_AddItemToArray(texCoord, x);
        cJSON_AddItemToArray(texCoord, y);
        cJSON_AddItemToArray(texCoords, texCoord);
    }
    cJSON_AddItemToObject(json, "texCoords", texCoords);
    return json;

}

void Sprite_Load(cJSON* json) {

    // Temporary value storage.
    char* spriteName;
    char* textureName;
    vec2 textureCoords[4];

    // Get the name of the sprite from the json object.
    cJSON* name = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (name == NULL || !cJSON_IsString(name)) {return;}
    spriteName = StringPool_Get(name->valuestring);

    // Get the texture name from the json object.
    cJSON* texture = cJSON_GetObjectItemCaseSensitive(json, "texture");
    if (texture == NULL || !cJSON_IsString(texture)) {return;}
    if (cJSON_IsNull(texture)) {textureName = NULL;}
    else {textureName = StringPool_Get(texture->valuestring);}

    // Get the texture coordinates from the json object.
    cJSON* texCoords = cJSON_GetObjectItemCaseSensitive(json, "texCoords");
    if (texCoords == NULL || !cJSON_IsArray(texCoords)) {return;}
    cJSON* texCoord = NULL;
    int coordsCount = 0;
    cJSON_ArrayForEach(texCoord, texCoords) {

        if (!cJSON_IsArray(texCoord)) {return;}        
        cJSON* value = NULL;
        int valueCount = 0;
        cJSON_ArrayForEach(value, texCoord) {
            if (!cJSON_IsNumber(value)) {return;}
            textureCoords[coordsCount][valueCount] = (float) value->valuedouble;
            valueCount++;
        }
        if (valueCount != 2) {return;}
        coordsCount++;

    }
    if (coordsCount != 4) {return;} 

    // Check if the sprite already exists.
    if (SpritePool_Has(spriteName)) {return;}
    
    // Verify the texture.
    Texture* t = NULL;
    if (textureName != NULL) {
        t = TexturePool_Get(textureName);
        if (t == NULL) {return;}
    } 
    
    // Fill the information of the new sprite.
    Sprite* s = malloc(sizeof(Sprite));
    s->name = spriteName;
    s->texture = t;
    for (int i = 0; i < 4; i++) {
        s->texCoords[i][0] = textureCoords[i][0];
        s->texCoords[i][1] = textureCoords[i][1];
    }

    // Add the sprite to the pool.
    SpritePool_Put(s);

}
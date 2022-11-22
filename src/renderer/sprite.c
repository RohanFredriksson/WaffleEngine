#include <stdlib.h>
#include <string.h>
#include "sprite.h"
#include "texture.h"
#include "assetpool.h"

void Sprite_Init(Sprite* s, Texture* t, char* name) {
    
    s->texture = t;
    if (s->texture != NULL) {    
        s->size[0] = s->texture->width;
        s->size[1] = s->texture->height;
    }

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

void Sprite_SetSize(Sprite* s, vec2 size) {
    glm_vec2_copy(size, s->size);
}

void Sprite_SetName(Sprite* s, char* name) {
    s->name = StringPool_Get(name);
}

bool Sprite_Equals(Sprite* s1, Sprite* s2) {

    // If the textures are not the same, return 0.
    if (s1->texture != s2->texture) {
        return 0;
    }
    
    // If the size is not the same, return 0.
    for (int i = 0; i < 2; i++) {
        if (s1->size[i] != s2->size[i]) {return 0;}
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

    cJSON* name = cJSON_CreateString(s->name);
    cJSON_AddItemToObject(json, "name", name);

    cJSON* texture;
    if (s->texture != NULL) {texture = cJSON_CreateString(s->texture->filename);}
    else {texture = cJSON_CreateString("NULL");}
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
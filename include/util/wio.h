#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"

#ifndef WIO_H
#define WIO_H

char* WIO_LoadSource(char* filename);
bool WIO_WriteFile(char* filename, char* contents);

bool WIO_ParseString(cJSON* json, char* name, char** dest);
bool WIO_ParseInt(cJSON* json, char* name, int* dest);
bool WIO_ParseBool(cJSON* json, char* name, bool* dest);
bool WIO_ParseFloat(cJSON* json, char* name, float* dest);
bool WIO_ParseVec2(cJSON* json, char* name, vec2 dest);
bool WIO_ParseVec3(cJSON* json, char* name, vec3 dest);
bool WIO_ParseVec4(cJSON* json, char* name, vec4 dest);
bool WIO_ParseMat4(cJSON* json, char* name, mat4 dest);

void WIO_AddString(cJSON* json, char* name, char* string);
void WIO_AddInt(cJSON* json, char* name, int value);
void WIO_AddBool(cJSON* json, char* name, bool value);
void WIO_AddFloat(cJSON* json, char* name, float value);
void WIO_AddVec2(cJSON* json, char* name, vec2 vector);
void WIO_AddVec3(cJSON* json, char* name, vec3 vector);
void WIO_AddVec4(cJSON* json, char* name, vec4 vector);
void WIO_AddMat4(cJSON* json, char* name, mat4 matrix);

#endif
#include "wio.h"
#include "assetpool.h"

char* WIO_LoadSource(char* filename) {
    
    FILE* f = fopen(filename, "r");
    if (f == NULL) {return NULL;}

    size_t size = 16;
    size_t length = 0;
    char* source = malloc(size);

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

bool WIO_WriteFile(char* filename, char* contents) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {return 0;}
    fprintf(file, contents);
    fclose(file);
    return 1;
}

bool WIO_ParseString(cJSON* json, char* name, char** dest) {

    cJSON* string = cJSON_GetObjectItemCaseSensitive(json, name);
    if (string == NULL) {return 0;}
    if (cJSON_IsNull(string)) {*dest = NULL; return 1;}
    if (!cJSON_IsString(string)) {return 0;}
    *dest = StringPool_Get(string->valuestring);

    return 1;
}

bool WIO_ParseInt(cJSON* json, char* name, int* dest) {

    cJSON* value = cJSON_GetObjectItemCaseSensitive(json, name);
    if (value == NULL || !cJSON_IsNumber(value)) {return 0;}
    *dest = value->valueint;

    return 1;
}

bool WIO_ParseBool(cJSON* json, char* name, bool* dest) {

    cJSON* value = cJSON_GetObjectItemCaseSensitive(json, name);
    if (value == NULL || !cJSON_IsBool(value)) {return 0;}
    *dest = value->valueint;

    return 1;

}

bool WIO_ParseFloat(cJSON* json, char* name, float* dest) {

    cJSON* value = cJSON_GetObjectItemCaseSensitive(json, name);
    if (value == NULL || !cJSON_IsNumber(value)) {return 0;}
    *dest = (float) value->valuedouble;

    return 1;
}

bool WIO_ParseVec2(cJSON* json, char* name, vec2 dest) {

    cJSON* vector = cJSON_GetObjectItemCaseSensitive(json, name);
    if (vector == NULL || !cJSON_IsArray(vector)) {return 0;}
    int i = 0;
    cJSON* value = NULL;
    cJSON_ArrayForEach(value, vector) {
        if (!cJSON_IsNumber(value) || i > 2) {return 0;}
        dest[i] = (float) value->valuedouble;
        i++;
    }

    return 1;
}

bool WIO_ParseVec3(cJSON* json, char* name, vec4 dest) {

    cJSON* vector = cJSON_GetObjectItemCaseSensitive(json, name);
    if (vector == NULL || !cJSON_IsArray(vector)) {return 0;}
    int i = 0;
    cJSON* value = NULL;
    cJSON_ArrayForEach(value, vector) {
        if (!cJSON_IsNumber(value) || i > 3) {return 0;}
        dest[i] = (float) value->valuedouble;
        i++;
    }

    return 1;
}


bool WIO_ParseVec4(cJSON* json, char* name, vec4 dest) {

    cJSON* vector = cJSON_GetObjectItemCaseSensitive(json, name);
    if (vector == NULL || !cJSON_IsArray(vector)) {return 0;}
    int i = 0;
    cJSON* value = NULL;
    cJSON_ArrayForEach(value, vector) {
        if (!cJSON_IsNumber(value) || i > 4) {return 0;}
        dest[i] = (float) value->valuedouble;
        i++;
    }

    return 1;
}


bool WIO_ParseMat4(cJSON* json, char* name, mat4 dest) {
    
    cJSON* matrix;
    cJSON* vector;
    cJSON* value;

    matrix = cJSON_GetObjectItemCaseSensitive(json, name);
    if (matrix == NULL || !cJSON_IsArray(matrix)) {return 0;}
    int i = 0;
    vector = NULL;
    cJSON_ArrayForEach(vector, matrix) {
        if (!cJSON_IsArray(vector) || i > 4) {return 0;}
        int j = 0;
        value = NULL;
        cJSON_ArrayForEach(value, vector) {
            if (!cJSON_IsNumber(value) || j > 4) {return 0;}
            dest[i][j] = (float) value->valuedouble;
            j++;
        }
        i++;
    }

    return 1;
}

void WIO_AddString(cJSON* json, char* name, char* string) {
    cJSON* str = cJSON_CreateString(string);
    cJSON_AddItemToObject(json, name, str);
}

void WIO_AddInt(cJSON* json, char* name, int value) {
    cJSON* integer = cJSON_CreateNumber(value);
    cJSON_AddItemToObject(json, name, integer);
}

void WIO_AddBool(cJSON* json, char* name, bool value) {
    cJSON* boolean = cJSON_CreateBool(value);
    cJSON_AddItemToObject(json, name, boolean);
}

void WIO_AddFloat(cJSON* json, char* name, float value) {
    cJSON* floating = cJSON_CreateNumber((float) value);
    cJSON_AddItemToObject(json, name, floating);
}

void WIO_AddVec2(cJSON* json, char* name, vec2 vector) {
    cJSON* vec = cJSON_CreateArray();
    cJSON* x = cJSON_CreateNumber((float) vector[0]);
    cJSON* y = cJSON_CreateNumber((float) vector[1]);
    cJSON_AddItemToArray(vec, x);
    cJSON_AddItemToArray(vec, y);
    cJSON_AddItemToObject(json, name, vec);
}

void WIO_AddVec3(cJSON* json, char* name, vec3 vector) {
    cJSON* vec = cJSON_CreateArray();
    cJSON* x = cJSON_CreateNumber((float) vector[0]);
    cJSON* y = cJSON_CreateNumber((float) vector[1]);
    cJSON* z = cJSON_CreateNumber((float) vector[2]);
    cJSON_AddItemToArray(vec, x);
    cJSON_AddItemToArray(vec, y);
    cJSON_AddItemToArray(vec, z);
    cJSON_AddItemToObject(json, name, vec);
}

void WIO_AddVec4(cJSON* json, char* name, vec4 vector)  {
    cJSON* vec = cJSON_CreateArray();
    cJSON* x = cJSON_CreateNumber((float) vector[0]);
    cJSON* y = cJSON_CreateNumber((float) vector[1]);
    cJSON* z = cJSON_CreateNumber((float) vector[2]);
    cJSON* w = cJSON_CreateNumber((float) vector[3]);
    cJSON_AddItemToArray(vec, x);
    cJSON_AddItemToArray(vec, y);
    cJSON_AddItemToArray(vec, z);
    cJSON_AddItemToArray(vec, w);
    cJSON_AddItemToObject(json, name, vec);
}

void WIO_AddMat4(cJSON* json, char* name, mat4 matrix) {
    cJSON* mat = cJSON_CreateArray();
    for (int i = 0; i < 4; i++) {
        cJSON* vec = cJSON_CreateArray();
        for (int j = 0; j < 4; j++) {
            cJSON* value = cJSON_CreateNumber((float) matrix[i][j]);
            cJSON_AddItemToArray(vec, value);
        }
        cJSON_AddItemToArray(mat, vec);
    }
    cJSON_AddItemToObject(json, name, mat);
}
#include "renderable.h"
#include "window.h"
#include "scene.h"
#include "renderer.h"

static void Renderable_Update(Component* c, float dt) {

    Renderable* r = (Renderable*) c->data;

    if (r->update != NULL) {r->update(r, dt);}

    if (!glm_vec4_eqv(r->colour, r->lastColour)) {
        glm_vec4_copy(r->colour, r->lastColour);
        r->isDirty = 1;
    }

    vec2 position;
    Component_GetPosition(c, position);
    if (!glm_vec2_eqv(position, r->lastPosition)) {
        glm_vec2_copy(position, r->lastPosition);
        r->isDirty = 1;
    }

    vec2 size;
    Component_GetSize(c, size);
    if (!glm_vec2_eqv(size, r->lastSize)) {
        glm_vec2_copy(size, r->lastSize);
        r->isDirty = 1;
    }

    float rotation = Component_GetRotation(c);
    if (rotation != r->lastRotation) {
        r->lastRotation = rotation;
        r->isDirty = 1;
    }

    if (r->zIndex != r->lastZIndex) {
        r->lastZIndex = r->zIndex;
        r->isDirty = 1;
    }

}

static cJSON* Renderable_Serialise(Component* c) {

    Renderable* r = (Renderable*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddVec4(json, "colour", r->colour);
    WIO_AddInt(json, "zIndex", r->zIndex);
    return json;

}

static void Renderable_Free(Component* c) {

    Renderable* r = (Renderable*) c->data;
    //Scene* scene = Window_GetScene();
    //Renderer_Remove(&scene->renderer, r);

    if (r->free != NULL) {r->free(r);}
    if (r->data != NULL) {free(r->data);}

}

static Renderable* _Renderable_Init(Component* c, 
                                    char* type,
                                    vec4 colour, 
                                    int zIndex) {

    Renderable* r = malloc(sizeof(Renderable));
    r->component = c;
    r->type = type;

    glm_vec4_copy(colour, r->colour);
    r->zIndex = zIndex;

    glm_vec4_copy(colour, r->lastColour);
    r->lastZIndex = zIndex;
    glm_vec2_zero(r->lastPosition);
    glm_vec2_zero(r->lastSize);
    r->lastRotation = 0.0f;
    
    r->isDirty = 1;

    c->data = r;
    c->update = &Renderable_Update;
    c->serialise = &Renderable_Serialise;
    c->free = &Renderable_Free;
    
    return r;
}

Component* Renderable_Init(char* type, vec4 colour, int zIndex) {
    Component* c = Component_Init("Renderable");
    _Renderable_Init(c, type, colour, zIndex);
    return c;
}

bool Renderable_Load(Component* c, cJSON* json) {

    char* type;
    int zIndex;
    vec4 colour;

    if (!WIO_ParseInt(json, "zIndex", &zIndex)) {return 0;}
    if (!WIO_ParseVec4(json, "colour", colour)) {return 0;}
    if (!WIO_ParseString(json, "type", &type)) {return 0;}

    // Initialise the renderable class.
    _Renderable_Init(c, type, colour, zIndex);

    return 1;
}

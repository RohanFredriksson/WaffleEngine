#include <stdbool.h>
#include "external.h"
#include "listener.h"
#include "entity.h"
#include "window.h"
#include "event.h"
#include "wmath.h"

#ifndef MOUSEBUTTON_H
#define MOUSEBUTTON_H

#define MOUSEBUTTON_CHECK_TRANSFORM 0
#define MOUSEBUTTON_CHECK_PICKINGTEXTURE 1
#define MOUSEBUTTON_EVENT_DOWN 0
#define MOUSEBUTTON_EVENT_BEGINDOWN 1

struct MouseButtonEvent {
    Event* event;
    int button;
    int checkType;
    int eventType;
};
typedef struct MouseButtonEvent MouseButtonEvent;

Component* MouseButton_Init(int button, int eventType, int checkType);

bool MouseButton_Check(Event* e, float dt);

cJSON* MouseButton_Serialise(Event* e);

bool MouseButton_Load(Event* e, cJSON* json);

#endif
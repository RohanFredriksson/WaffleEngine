#include <stdbool.h>
#include "listener.h"
#include "entity.h"
#include "window.h"
#include "event.h"
#include "wmath.h"

#ifndef MOUSEBUTTON_H
#define MOUSEBUTTON_H

struct MouseButtonEvent {
    Event* event;
    int button;
    bool beginDown;
    bool usePickingTexture;
};
typedef struct MouseButtonEvent MouseButtonEvent;

Component* MouseButton_Init(int button, bool beginDown, bool usePickingTexture);

Component* MouseButtonDown_Init(int button, bool usePickingTexture);

Component* MouseButtonBeginDown_Init(int button, bool usePickingTexture);

bool MouseButton_Check(Event* e, float dt);

#endif
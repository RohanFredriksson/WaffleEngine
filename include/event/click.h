#include <stdbool.h>
#include "listener.h"
#include "gameobject.h"
#include "window.h"
#include "event.h"
#include "wmath.h"

#ifndef CLICK_H
#define CLICK_H

struct Click {
    Event* event;
    int button;
    bool usePickingTexture;
};
typedef struct Click Click;

Component* Click_Init(int button, bool usePickingTexture);

bool Click_Check(Event* e, float dt);

void Click_Free(Event* e);

#endif
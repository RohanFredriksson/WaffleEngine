#include "mousebutton.h"

Component* MouseButton_Init(int button, int eventType, int checkType) {

    Component* component = Event_Init("MouseButtonDown");
    
    Event* event = (Event*) component->data;

    MouseButtonEvent* mb = malloc(sizeof(MouseButtonEvent));
    mb->event = event;
    mb->button = button;
    mb->checkType = checkType;
    mb->eventType = eventType;

    event->check = &MouseButton_Check;
    event->serialise = &MouseButton_Serialise;
    event->data = mb;

    return component;
}

bool MouseButton_Check(Event* e, float dt) {

    MouseButtonEvent* mb = (MouseButtonEvent*) e->data;
    
    bool flag;
    if (mb->eventType == MOUSEBUTTON_EVENT_DOWN) {flag = MouseListener_MouseButtonDown(mb->button);}
    else {flag = MouseListener_MouseButtonBeginDown(mb->button);}

    if (flag) {

        if (mb->checkType == MOUSEBUTTON_CHECK_PICKINGTEXTURE) {

            int x = MouseListener_GetX();
            int y = MouseListener_GetY();
            int id = Window_ReadPixel(x, y);

            if (id == e->component->entity->id) {return 1;}

        }

        else {

            vec2 position;
            vec2 size;
            Component_GetPosition(e->component, position);
            Component_GetSize(e->component, size);
            float rotation = Component_GetRotation(e->component);

            float left = position[0] - size[0] / 2.0f;
            float right = position[0] + size[0] / 2.0f;
            float bottom = position[1] - size[1] / 2.0f;
            float top = position[1] + size[1] / 2.0f;
            
            vec2 m = { (float) MouseListener_GetWorldX(), (float) MouseListener_GetWorldY() };
            WMath_Rotate(m, rotation, position);

            if (m[0] >= left && m[0] <= right && m[1] >= bottom && m[1] <= top) {return 1;}

        }

    }

    return 0;
}

cJSON* MouseButton_Serialise(Event* e) {

    MouseButtonEvent* mb = (MouseButtonEvent*) e->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "button", mb->button);
    WIO_AddInt(json, "checkType", mb->checkType);
    WIO_AddInt(json, "eventType", mb->eventType);
    return json;

}
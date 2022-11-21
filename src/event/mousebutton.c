#include "mousebutton.h"

Component* MouseButton_Init(int button, bool beginDown, bool usePickingTexture) {

    Component* component = Event_Init("MouseButtonDown", &MouseButton_Check, NULL, NULL);
    Event* event = (Event*) component->data;
    MouseButtonEvent* mb = malloc(sizeof(MouseButtonEvent));

    mb->event = event;
    mb->button = button;
    mb->beginDown = beginDown;
    mb->usePickingTexture = usePickingTexture;

    event->data = mb;
    return component;

}

Component* MouseButtonDown_Init(int button, bool usePickingTexture) {
    return MouseButton_Init(button, 0, usePickingTexture);
}

Component* MouseButtonBeginDown_Init(int button, bool usePickingTexture) {
    return MouseButton_Init(button, 1, usePickingTexture);
}

bool MouseButton_Check(Event* e, float dt) {

    MouseButtonEvent* mb = (MouseButtonEvent*) e->data;
    
    bool flag;
    if (mb->beginDown) {flag = MouseListener_MouseButtonBeginDown(mb->button);}
    else {flag = MouseListener_MouseButtonDown(mb->button);}

    if (flag) {

        if (mb->usePickingTexture) {

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
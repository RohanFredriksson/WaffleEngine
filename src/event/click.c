#include "click.h"

Component* Click_Init(int button, bool usePickingTexture) {

    Component* component = Event_Init("Click", &Click_Check, NULL, NULL);
    Event* event = (Event*) component->data;
    Click* click = malloc(sizeof(Click));

    click->event = event;
    click->button = button;
    click->usePickingTexture = usePickingTexture;

    event->data = click;
    return component;

}

bool Click_Check(Event* e, float dt) {

    Click* c = (Click*) e->data;
    if (MouseListener_MouseButtonDown(c->button)) {

        if (c->usePickingTexture) {

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
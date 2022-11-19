#include "click.h"

Component* Click_Init(int button, bool usePickingTexture) {

    Component* component = Event_Init("Click", &Click_Check, &Click_Free);
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

            if (id == e->component->go->id) {return 1;}

        }

        else {

            Transform* transform = e->component->go->transform;
            float left = transform->pos[0] - transform->size[0] / 2.0f;
            float right = transform->pos[0] + transform->size[0] / 2.0f;
            float bottom = transform->pos[1] - transform->size[1] / 2.0f;
            float top = transform->pos[1] + transform->size[1] / 2.0f;
            
            vec2 m = { (float) MouseListener_GetWorldX(), (float) MouseListener_GetWorldY() };
            WMath_Rotate(m, transform->rotation, transform->pos);

            if (m[0] >= left && m[0] <= right && m[1] >= bottom && m[1] <= top) {return 1;}

        }

    }
    
    return 0;
}

void Click_Free(Event* e) {

}
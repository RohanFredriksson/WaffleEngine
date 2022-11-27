#include "title.h"
#include "assetpool.h"
#include "cameracontroller.h"
#include "rigidbody.h"
#include "primitives.h"
#include "spritesheet.h"

#include "event.h"
#include "events.h"
#include "command.h"
#include "commands.h"

void Title_Init(Scene* s) {

    Spritesheet_Load("assets/textures/faces.png");

    // Armaan
    Entity* entityArmaan = Entity_Init((vec2){ 0.0f, 0.0f }, (vec2){ 1, 1 }, 0);

    Sprite* spriteArmaan = SpritePool_Get("armaan");
    Component* rigidbodyArmaan = Rigidbody_Init();
    Component* colliderArmaan = Box_Init((vec2) {5.0f, 1.0f});
    Rigidbody_SetCollider(rigidbodyArmaan, colliderArmaan);
    Rigidbody_SetMass(rigidbodyArmaan, 0.0f);
    Collider_SetRigidbody(colliderArmaan, rigidbodyArmaan);
    Component* spriteRendererArmaan = SpriteRenderer_Init(spriteArmaan, (vec4){ 1, 1, 1, 1 }, 0);
    Component* cameraControllerArmaan = CameraController_Init(NULL);
    Command* commandArmaan = MoveCamera_Init((vec2) { 1.0f, -1.5f }, 1.0f);
    Component* eventArmaan = MouseButton_Init(GLFW_MOUSE_BUTTON_LEFT, MOUSEBUTTON_EVENT_BEGINDOWN, MOUSEBUTTON_CHECK_PICKINGTEXTURE);
    Event_AddCommand(eventArmaan, commandArmaan);

    Entity_AddComponent(entityArmaan, spriteRendererArmaan);
    Entity_AddComponent(entityArmaan, rigidbodyArmaan);
    Entity_AddComponent(entityArmaan, colliderArmaan);
    Entity_AddComponent(entityArmaan, cameraControllerArmaan);
    Entity_AddComponent(entityArmaan, eventArmaan);
    Scene_AddEntity(s, entityArmaan);

    // Ainsley
    Entity* entityAinsley = Entity_Init((vec2){ -4.0f, 0.0f }, (vec2){ 1, 1 }, 0);

    Sprite* spriteAinsley = SpritePool_Get("ainsley");
    Component* rigidbodyAinsley = Rigidbody_Init();
    Component* colliderAinsley = Circle_Init(0.5f);
    Rigidbody_SetCollider(rigidbodyAinsley, colliderAinsley);
    Rigidbody_SetVelocity(rigidbodyAinsley, (vec2) { 1.0f, 10.0f });
    Rigidbody_SetCor(rigidbodyAinsley, 0.75f);
    Collider_SetRigidbody(colliderAinsley, rigidbodyAinsley);
    Component* spriteRendererAinsley = SpriteRenderer_Init(spriteAinsley, (vec4){ 1, 1, 1, 1 }, 0);
    Command* commandAinsley = DeleteThisEntity_Init();
    Component* eventAinsley = Trigger_Init();
    Event_AddCommand(eventAinsley, commandAinsley);
    Event_SetMulti(eventAinsley, EVENT_MULTIPLE_USE);

    Entity_AddComponent(entityAinsley, rigidbodyAinsley);
    Entity_AddComponent(entityAinsley, colliderAinsley);
    Entity_AddComponent(entityAinsley, spriteRendererAinsley);
    Entity_AddComponent(entityAinsley, eventAinsley);
    Scene_AddEntity(s, entityAinsley);

}
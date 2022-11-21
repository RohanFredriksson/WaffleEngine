#include "title.h"
#include "assetpool.h"
#include "cameracontroller.h"
#include "rigidbody.h"
#include "primitives.h"
#include "spritesheet.h"

#include "movecamera.h"
#include "console.h"
#include "mousebutton.h"
#include "trigger.h"
#include "command.h"

void Title_Init(Scene* s) {

    Spritesheet_Load("assets/textures/faces.png");

    // Armaan
    Sprite* spriteArmaan = SpritePool_Get("armaan");
    Component* rigidbodyArmaan = Rigidbody_Init();
    Component* colliderArmaan = Box_Init((vec2) {5.0f, 1.0f}, rigidbodyArmaan);
    Rigidbody_SetCollider(rigidbodyArmaan, colliderArmaan);
    Rigidbody_SetMass(rigidbodyArmaan, 0.0f);
    Component* spriteRendererArmaan = SpriteRenderer_Init(spriteArmaan, (vec4){ 1, 1, 1, 1 }, 0);
    Component* cameraControllerArmaan = CameraController_Init(NULL);
    Command* commandArmaan = MoveCamera_Init((vec2) { 1.0f, -1.5f }, 1.0f);
    Component* eventArmaan = MouseButtonBeginDown_Init(GLFW_MOUSE_BUTTON_LEFT, 1);
    Event_AddCommand(eventArmaan, commandArmaan);
    
    Entity* entityArmaan = Entity_Init((vec2){ 0.0f, 0.0f }, (vec2){ 1, 1 }, 0);

    Entity_AddComponent(entityArmaan, spriteRendererArmaan);
    Entity_AddComponent(entityArmaan, rigidbodyArmaan);
    Entity_AddComponent(entityArmaan, colliderArmaan);
    Entity_AddComponent(entityArmaan, cameraControllerArmaan);
    Entity_AddComponent(entityArmaan, eventArmaan);
    Scene_AddEntity(s, entityArmaan);

    // Ainsley
    Sprite* spriteAinsley = SpritePool_Get("ainsley");
    Component* rigidbodyAinsley = Rigidbody_Init();
    Component* colliderAinsley = Circle_Init(0.5f, rigidbodyAinsley);
    Rigidbody_SetCollider(rigidbodyAinsley, colliderAinsley);
    Rigidbody_SetVelocity(rigidbodyAinsley, (vec2) { 1.0f, 10.0f });
    Rigidbody_SetCor(rigidbodyAinsley, 0.75f);
    Component* spriteRendererAinsley = SpriteRenderer_Init(spriteAinsley, (vec4){ 1, 1, 1, 1 }, 0);
    Command* commandAinsley = Console_Init("COLLISION\n");
    Component* eventAinsley = Trigger_Init();
    Event_AddCommand(eventAinsley, commandAinsley);
    Event_SetMulti(eventAinsley, 1);

    Entity* entityAinsley = Entity_Init((vec2){ -4.0f, 0.0f }, (vec2){ 1, 1 }, 0);

    Entity_AddComponent(entityAinsley, rigidbodyAinsley);
    Entity_AddComponent(entityAinsley, colliderAinsley);
    Entity_AddComponent(entityAinsley, spriteRendererAinsley);
    Entity_AddComponent(entityAinsley, eventAinsley);
    Scene_AddEntity(s, entityAinsley);

}
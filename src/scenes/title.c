#include "title.h"
#include "assetpool.h"
#include "cameracontroller.h"
#include "rigidbody.h"
#include "primitives.h"

#include "command.h"
#include "click.h"
#include "trigger.h"
#include "print.h"

void Title_Init(Scene* s) {

    // Armaan
    Sprite* spriteArmaan = SpritePool_Get("./assets/textures/armaan.png");
    Transform* transformArmaan = TransformPool_Add((vec2){ 0.0f, 0.0f }, (vec2){ 1, 1 }, 0);
    
    Component* rigidbodyArmaan = Rigidbody_Init(transformArmaan);
    Component* colliderArmaan = Box_Init((vec2) {5.0f, 1.0f}, rigidbodyArmaan);
    Rigidbody_SetCollider(rigidbodyArmaan, colliderArmaan);
    Rigidbody_SetMass(rigidbodyArmaan, 0.0f);
    Component* spriteRendererArmaan = SpriteRenderer_Init(spriteArmaan, (vec4){ 1, 1, 1, 1 }, transformArmaan, 0);
    Component* cameraControllerArmaan = CameraController_Init(NULL);
    GameObject* gameObjectArmaan = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectArmaan, spriteRendererArmaan);
    GameObject_AddComponent(gameObjectArmaan, rigidbodyArmaan);
    GameObject_AddComponent(gameObjectArmaan, colliderArmaan);
    GameObject_AddComponent(gameObjectArmaan, cameraControllerArmaan);
    Scene_AddGameObject(s, gameObjectArmaan);

    // Ainsley
    Sprite* spriteAinsley = SpritePool_Get("./assets/textures/ainsley.jpg");
    Transform* transformAinsley = TransformPool_Add((vec2){ -4.0f, 0.0f }, (vec2){ 1, 1 }, 0);

    Component* rigidbodyAinsley = Rigidbody_Init(transformAinsley);
    Component* colliderAinsley = Circle_Init(0.5f, rigidbodyAinsley);
    Rigidbody_SetCollider(rigidbodyAinsley, colliderAinsley);
    Rigidbody_SetVelocity(rigidbodyAinsley, (vec2) { 1.0f, 10.0f });
    Rigidbody_SetCor(rigidbodyAinsley, 0.75f);
    
    Component* spriteRendererAinsley = SpriteRenderer_Init(spriteAinsley, (vec4){ 1, 1, 1, 1 }, transformAinsley, 0);

    Command* commandAinsley = Print_Init("COLLISION\n");
    Component* eventAinsley = Trigger_Init();
    Event_AddCommand(eventAinsley, commandAinsley);
    Event_SetMulti(eventAinsley, 1);

    GameObject* gameObjectAinsley = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectAinsley, rigidbodyAinsley);
    GameObject_AddComponent(gameObjectAinsley, colliderAinsley);
    GameObject_AddComponent(gameObjectAinsley, spriteRendererAinsley);
    GameObject_AddComponent(gameObjectAinsley, eventAinsley);
    Scene_AddGameObject(s, gameObjectAinsley);

}
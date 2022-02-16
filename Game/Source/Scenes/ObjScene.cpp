#include "Framework.h"

#include "ObjScene.h"
#include "DataTypes.h"
#include "GameObjects/Cube.h"
#include "Game.h"

ObjScene::ObjScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
    vec3 cameraOffset = vec3(0.f, 0.f, -12.f);

    m_pCamera = new fw::Camera(this, centerOfScreen + cameraOffset);

    fw::GameObject* pObj= new fw::GameObject(this, centerOfScreen + vec3(0.f, -3.5f,0.f), vec3());
    pObj->AddComponent(new fw::MeshComponent(pGame->GetMesh("Obj"), pGame->GetMaterial("Arcade_Cabinet")));
    m_Objects.push_back(pObj);

    pGame->GetMesh("Obj")->LoadObj("Data/Models/Arcade_Cabinet.obj", true);
}

ObjScene::~ObjScene()
{
}

void ObjScene::StartFrame(float deltaTime)
{
}

void ObjScene::OnEvent(fw::Event* pEvent)
{
}

void ObjScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

    float time = (float)fw::GetSystemTimeSinceGameStart() * 10;
    m_Objects[0]->SetRotation(vec3(0.f, time, 0.f));
}

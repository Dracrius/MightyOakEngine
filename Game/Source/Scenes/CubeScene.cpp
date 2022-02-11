#include "Framework.h"

#include "CubeScene.h"
#include "DataTypes.h"
#include "GameObjects/Cube.h"
#include "Game.h"

CubeScene::CubeScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
    vec3 cameraOffset = vec3(0.f, 0.f, -20.f);

    m_pCamera = new fw::Camera(this, centerOfScreen + cameraOffset);

    fw::GameObject* pCube = new fw::GameObject(this, centerOfScreen, vec3());
    pCube->AddComponent(new fw::MeshComponent(pGame->GetMesh("Cube"), pGame->GetMaterial("Cube")));
    m_Objects.push_back(pCube);
}

CubeScene::~CubeScene()
{
}

void CubeScene::StartFrame(float deltaTime)
{
}

void CubeScene::OnEvent(fw::Event* pEvent)
{
}

void CubeScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

    float time = (float)fw::GetSystemTimeSinceGameStart() * 10;
    m_Objects[0]->SetRotation(vec3(time, time, time));
}

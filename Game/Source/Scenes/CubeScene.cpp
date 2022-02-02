#include "Framework.h"

#include "CubeScene.h"
#include "DataTypes.h"
#include "GameObjects/Cube.h"
#include "Game.h"

CubeScene::CubeScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    m_pCamera = new fw::Camera(this, vec3(vec2(1.5f * 10, 1.5f * 10) / 2) + vec3(0.f, 0.f, -20.f));

    Cube* pPlatform = new Cube(this, pGame->GetMesh("Cube"), pGame->GetMaterial("Cube"), vec2(1.5f * 10, 1.5f * 10) / 2, vec3());
    //pPlatform->CreateBody(m_pPhysicsWorld, false, vec3(1.f, 1.f, 1.f), 1.f);
    m_Objects.push_back(pPlatform);
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

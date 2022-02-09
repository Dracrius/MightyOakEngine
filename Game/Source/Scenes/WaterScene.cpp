#include "Framework.h"

#include "WaterScene.h"
#include "DataTypes.h"
#include "GameObjects/Player.h"
#include "GameObjects/Cube.h"
#include "Game.h"

WaterScene::WaterScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
    vec3 cameraOffset = vec3(0.f, 0.f, -20.f);

    m_pCamera = new fw::Camera(this, centerOfScreen + cameraOffset);

    vec3 rot = vec3(0.f, 0.f, 0.f);
    vec3 pos = centerOfScreen + vec3(0.f,-1.f,0.f);

    Cube* pPlane = new Cube(this, pGame->GetMesh("Plane"), pGame->GetMaterial("Water"), pos, rot);
    m_Objects.push_back(pPlane);
}

WaterScene::~WaterScene()
{
}

void WaterScene::StartFrame(float deltaTime)
{
}

void WaterScene::OnEvent(fw::Event* pEvent)
{
}

void WaterScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

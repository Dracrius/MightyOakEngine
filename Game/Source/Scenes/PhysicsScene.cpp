#include "Framework.h"

#include "PhysicsScene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Player.h"
#include "GameObjects/Cube.h"
#include "Game.h"

PhysicsScene::PhysicsScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
    vec3 cameraOffset = vec3(0.f, 0.f, -20.f);

    m_pCamera = new fw::Camera(this, centerOfScreen + cameraOffset);

    m_pPlayerController = new PlayerController();

    Player* pPlayer = new Player(this, pGame->GetMesh("Sprite"), pGame->GetMaterial("Sokoban"), vec2(7.5f, 12.0f), m_pPlayerController);
    pPlayer->SetSpriteSheet(pGame->GetSpriteSheet("Sprites"));
    pPlayer->CreateBody(m_pPhysicsWorld, true, 1.f);
    m_Objects.push_back(pPlayer);

    m_pCamera->AttachTo(m_Objects.front());
    m_pCamera->SetThirdPerson(cameraOffset);

    Cube* pBox = new Cube(this, pGame->GetMesh("Cube"), pGame->GetMaterial("Cube"), centerOfScreen, vec3());
    pBox->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
    m_Objects.push_back(pBox);
}

PhysicsScene::~PhysicsScene()
{
    delete m_pPlayerController;
}

void PhysicsScene::StartFrame(float deltaTime)
{
    m_pPlayerController->StartFrame();
}

void PhysicsScene::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent(pEvent);

    fw::Scene::OnEvent(pEvent);
}

void PhysicsScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

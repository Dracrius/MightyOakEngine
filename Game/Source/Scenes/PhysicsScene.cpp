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

    m_pCamera = new fw::Camera(this, vec3(vec2(1.5f * 10, 1.5f * 10) / 2) + vec3(0.f, 0.f, -20.f));

    m_pPlayerController = new PlayerController();

    Player* pPlayer = new Player(this, pGame->GetMesh("Sprite"), pGame->GetMaterial("Sokoban"), vec2(7.0f, 9.0f), m_pPlayerController);
    pPlayer->SetSpriteSheet(pGame->GetSpriteSheet("Sprites"));
    pPlayer->CreateBody(m_pPhysicsWorld, true, vec2(1.f, 1.f), 1.f);
    m_Objects.push_back(pPlayer);

    Cube* pPlatform = new Cube(this, pGame->GetMesh("Cube"), pGame->GetMaterial("Cube"), vec2(7.0f, 4.0f), vec3());
    pPlatform->CreateBody(m_pPhysicsWorld, false, vec3(1.f, 1.f, 1.f), 1.f);
    m_Objects.push_back(pPlatform);
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

#include "Framework.h"

#include "PhysicsScene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Player.h"
#include "Game.h"
#include "DefaultSettings.h"

PhysicsScene::PhysicsScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
    m_pPhysicsWorld->SetGravity(c_gravity);

    m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);

    m_pPlayerController = new PlayerController();

    Player* pPlayer = new Player(this, pGame->GetMesh("Sprite"), pGame->GetMaterial("Sokoban"), vec2(7.5f, 12.0f), m_pPlayerController);
    pPlayer->SetSpriteSheet(pGame->GetSpriteSheet("Sprites"));
    pPlayer->CreateBody(m_pPhysicsWorld, true, 1.f);
    m_Objects.push_back(pPlayer);

    m_pCamera->AttachTo(m_Objects.front());
    m_pCamera->SetThirdPerson(c_cameraOffset);

    fw::GameObject* pBox = new fw::GameObject(this, c_centerOfScreen, vec3());
    pBox->AddComponent(new fw::MeshComponent(pGame->GetMesh("Cube"), pGame->GetMaterial("Cube")));
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

	ControlsMenu();
}

void PhysicsScene::ControlsMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Reset Background Color", "Ctrl+R"))
				{
					Game* pGame = static_cast<Game*>(m_pGame);

					pGame->ResetBackgroundColor(false);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Controls"))
		{
			ImGui::Text("Left = <- or A");
			ImGui::Text("Right = -> or D");
			ImGui::Text("Jump = Up or Space");
			ImGui::Text("Teleport = Z");

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Physics Scene", NULL, false, false);

		ImGui::EndMainMenuBar();
	}
}

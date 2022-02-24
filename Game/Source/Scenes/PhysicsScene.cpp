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

	vec3 pos = c_centerOfScreen + vec3(0.f, 0.f, 2.f);
	vec3 rot = vec3(0.f, 0.f, 0.f);

	fw::GameObject* pBackground = new fw::GameObject(this, pos, rot);
	pBackground->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("Background")));
	pBackground->SetScale(vec3(20.f));
	pBackground->SetName("Background");
	m_Objects.push_back(pBackground);

    Player* pPlayer = new Player(this, m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("Sokoban"), vec2(7.5f, 12.0f), m_pPlayerController);
    pPlayer->SetSpriteSheet(m_pResourceManager->GetSpriteSheet("Sprites"));
    pPlayer->CreateBody(m_pPhysicsWorld, true, vec3(c_playerCollider.x, c_playerCollider.y, c_playerCollider.y), 1.f);
	pPlayer->SetName("Player");
    m_Objects.push_back(pPlayer);

    m_pCamera->AttachTo(m_Objects[1]);
    m_pCamera->SetThirdPerson(c_cameraOffset);

    fw::GameObject* pBox = new fw::GameObject(this, c_centerOfScreen, vec3());
    pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
    pBox->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
	pBox->SetName("Box Platform");
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
		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Enable Debug Draw", "", &m_debugDraw)) {}

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

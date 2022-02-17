#include "Framework.h"

#include "CubeScene.h"
#include "DataTypes.h"
#include "GameObjects/Cube.h"
#include "Game.h"

CubeScene::CubeScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
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

	vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;

	vec3 rot = m_Objects[0]->GetRotation();
	float offset = 0.f;

	Game* pGame = static_cast<Game*>(m_pGame);

	m_openRot[0] = m_Objects[0]->GetRotation().x;
	m_openRot[1] = m_Objects[0]->GetRotation().y;
	m_openRot[2] = m_Objects[0]->GetRotation().z;

	if (m_showSlider)
	{
		ImGui::SetNextWindowSize(ImVec2(180, 60), ImGuiCond_Always);
		if (!ImGui::Begin("Rotation Slider", &m_showSlider))
		{
			ImGui::End();
			return;
		}

		ImGui::SliderFloat3("Angle", m_openRot, 0.f, 359.9f);

		ImGui::End();
	}

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
			if (ImGui::MenuItem("Auto Rotation", "", &m_autoRotate)) {}
			ImGui::InputInt("Speed", &m_rotationSpeed, 1, 5);

			ImGui::MenuItem("Show Rotation Slider", "", &m_showSlider);

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Cube Scene", NULL, false, false);
		
		ImGui::EndMainMenuBar();
	}

	if (m_autoRotate)
	{
		offset = m_rotationSpeed * deltaTime;
	}

    m_Objects[0]->SetRotation(vec3(m_openRot[0] + offset, m_openRot[1] + offset, m_openRot[2] + offset));
}

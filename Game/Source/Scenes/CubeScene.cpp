#include "Framework.h"

#include "CubeScene.h"
#include "DataTypes.h"
#include "Game.h"
#include "DefaultSettings.h"

CubeScene::CubeScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
    m_pPhysicsWorld->SetGravity(c_gravity);

    m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);
	m_pCamera->SetAspectRatio(c_aspectRatio);

    fw::GameObject* pCube = new fw::GameObject(this, c_centerOfScreen, vec3());
    pCube->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
	pCube->SetName("Numbered Cube");
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

	vec3 rot = m_Objects[0]->GetRotation();
	float offset = 0.f;

	Game* pGame = static_cast<Game*>(m_pGame);

	m_openRot[0] = m_Objects[0]->GetRotation().x;
	m_openRot[1] = m_Objects[0]->GetRotation().y;
	m_openRot[2] = m_Objects[0]->GetRotation().z;

	if (m_showSliders)
	{
		Sliders();
	}

	SettingsMenu();

	if (m_autoRotate)
	{
		offset = m_rotationSpeed * deltaTime;
	}

    m_Objects[0]->SetRotation(vec3(m_openRot[0] + offset, m_openRot[1] + offset, m_openRot[2] + offset));
}

void CubeScene::Sliders()
{
	ImGui::SetNextWindowSize(ImVec2(180, 60), ImGuiCond_Always);
	if (!ImGui::Begin("Rotation Slider", &m_showSliders))
	{
		ImGui::End();
		return;
	}

	ImGui::SliderFloat3("Angle", m_openRot, 0.f, 359.9f);

	ImGui::End();
}

void CubeScene::SettingsMenu()
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
			if (ImGui::MenuItem("Auto Rotation", "", &m_autoRotate)) {}
			ImGui::InputInt("Speed", &m_rotationSpeed, 1, 5);

			ImGui::MenuItem("Show Rotation Slider", "", &m_showSliders);

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Cube Scene", NULL, false, false);

		ImGui::EndMainMenuBar();
	}
}

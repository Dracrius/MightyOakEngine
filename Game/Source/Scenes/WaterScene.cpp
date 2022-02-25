#include "Framework.h"

#include "WaterScene.h"
#include "DataTypes.h"
#include "GameObjects/Player.h"
#include "Game.h"
#include "DefaultSettings.h"

WaterScene::WaterScene(Game* pGame) : fw::Scene(pGame)
{

    m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
    m_pPhysicsWorld->SetGravity(c_gravity);

    m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);
	m_pCamera->SetAspectRatio(c_aspectRatio);

    vec3 rot = vec3();
    vec3 pos = c_centerOfScreen + vec3(0.f,-1.f,0.f);

    fw::GameObject* pPlane = new fw::GameObject(this, pos, rot);
    pPlane->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Plane"), m_pResourceManager->GetMaterial("Water")));
	pPlane->SetName("Water");
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

	if (m_showSlider)
	{
		Sliders();
	}

	SettingsMenu();
}

void WaterScene::Sliders()
{
	ImGui::SetNextWindowSize(ImVec2(400, 80), ImGuiCond_Always);
	if (!ImGui::Begin("Plane Settings", &m_showSlider))
	{
		ImGui::End();
		return;
	}

	float lastSize[2] = { m_planeSize[0], m_planeSize[1] };
	int lastVertRes[2] = { m_planeVertRes[0], m_planeVertRes[1] };

	ImGui::SliderFloat2("Plane Size", m_planeSize, 1.f, 200.0f);
	ImGui::SliderInt2("Vertex Resolution", m_planeVertRes, 2, 2000);

	if (m_planeSize[0] != lastSize[0] || m_planeSize[1] != lastSize[1] || m_planeVertRes[0] != lastVertRes[0] || m_planeVertRes[1] != lastVertRes[1])
	{
		m_pResourceManager->GetMesh("Plane")->CreatePlane(vec2(m_planeSize[0], m_planeSize[1]), ivec2(m_planeVertRes[0], m_planeVertRes[1]));
	}
	ImGui::End();
}

void WaterScene::SettingsMenu()
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
		if (ImGui::MenuItem("Settings", "", &m_showSlider)) {};

		ImGui::MenuItem("Cube Scene", NULL, false, false);
		ImGui::EndMainMenuBar();
	}
}

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

    fw::GameObject* pPlane = new fw::GameObject(this, pos, rot);
    pPlane->AddComponent(new fw::MeshComponent(pGame->GetMesh("Plane"), pGame->GetMaterial("Water")));
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
			Game* pGame = static_cast<Game*>(m_pGame);

			pGame->GetMesh("Plane")->CreatePlane(vec2(m_planeSize[0], m_planeSize[1]), ivec2(m_planeVertRes[0], m_planeVertRes[1]));
		}
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
		if (ImGui::MenuItem("Settings", "", &m_showSlider)) {};

		ImGui::MenuItem("Cube Scene", NULL, false, false);
		ImGui::EndMainMenuBar();
	}

}

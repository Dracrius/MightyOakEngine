#include "Framework.h"

#include "ObjScene.h"
#include "DataTypes.h"
#include "GameObjects/Cube.h"
#include "Game.h"

ObjScene::ObjScene(Game* pGame) : fw::Scene(pGame)
{
	char filename[260] = "Data/Models/Arcade_Cabinet.obj";
	strcpy_s(m_filename, filename);
	char textureName[260] = "Data/Textures/Arcade_Cabinet.png";
	strcpy_s(m_textureName, textureName);

    m_pPhysicsWorld = new fw::PhysicsWorldBox2D();
    m_pPhysicsWorld->SetGravity(vec2(0.f, -9.8f));

    vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;
    vec3 cameraOffset = vec3(0.f, 0.f, -12.f);

	m_openPos[0] = centerOfScreen.x;
	m_openPos[1] = centerOfScreen.y - 3.5f;
	m_openPos[2] = centerOfScreen.z;

    m_pCamera = new fw::Camera(this, centerOfScreen + cameraOffset);

	m_lastObj = "Data/Models/Arcade_Cabinet.obj";
	m_lastTexture = "Data/Textures/Arcade_Cabinet.png";

    fw::GameObject* pObj= new fw::GameObject(this, centerOfScreen + vec3(0.f, -3.5f,0.f), vec3());
    pObj->AddComponent(new fw::MeshComponent(pGame->GetMesh("Obj"), pGame->GetMaterial("Arcade_Cabinet")));
    m_Objects.push_back(pObj);

    pGame->GetMesh("Obj")->LoadObj(m_lastObj, true);

    fw::GameObject* pFloor = new fw::GameObject(this, centerOfScreen + vec3(0.f, -3.5f, 0.f), vec3(-90.f, 0.f, 0.f));
    pFloor->AddComponent(new fw::MeshComponent(pGame->GetMesh("Sprite"), pGame->GetMaterial("Arcade_Floor")));
    pFloor->SetScale(vec3(28.f));
    m_Objects.push_back(pFloor);

}

ObjScene::~ObjScene()
{
}

void ObjScene::StartFrame(float deltaTime)
{
}

void ObjScene::OnEvent(fw::Event* pEvent)
{
}

void ObjScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

	vec3 centerOfScreen = vec2(1.5f * 10, 1.5f * 10) / 2;

	float rot = m_Objects[0]->GetRotation().y;
	float offset = 0.f;

	Game* pGame = static_cast<Game*>(m_pGame);

	fw::MeshComponent* pMeshComponent = static_cast<fw::MeshComponent*>(m_Objects[0]->GetComponent(fw::MeshComponent::GetStaticType()));

	if (m_showSlider)
	{
		ImGui::SetNextWindowSize(ImVec2(180, 60), ImGuiCond_Always);
		if (!ImGui::Begin("Rotation Slider", &m_showSlider))
		{
			ImGui::End();
			return;
		}

		ImGui::SliderFloat("Angle", &rot, 0.f, 359.9f);

		ImGui::End();
	}

	if (m_showOpenObj)
	{
		ImGui::SetNextWindowSize(ImVec2(410, 210), ImGuiCond_Always);
		if (!ImGui::Begin("Open Obj", &m_showOpenObj))
		{
			ImGui::End();
			return;
		}

		m_autoRotate = false;

		m_openPos[0] = m_Objects[0]->GetPosition().x;
		m_openPos[1] = m_Objects[0]->GetPosition().y;
		m_openPos[2] = m_Objects[0]->GetPosition().z;
		m_openRot[0] = m_Objects[0]->GetRotation().x;
		m_openRot[1] = m_Objects[0]->GetRotation().y;
		m_openRot[2] = m_Objects[0]->GetRotation().z;
		m_openScale[0] = m_Objects[0]->GetScale().x;
		m_openScale[1] = m_Objects[0]->GetScale().y;
		m_openScale[2] = m_Objects[0]->GetScale().z;

		ImGui::InputTextWithHint("Filename", "Data/Models/filename.obj", m_filename, IM_ARRAYSIZE(m_filename));
		if (!ImGui::IsAnyItemActive())
			ImGui::SetKeyboardFocusHere();
		ImGui::MenuItem("Does it have a Texture?", "", &m_hasTexture);
		ImGui::InputTextWithHint("Texture", "Data/Textures/filename.ext", m_textureName, IM_ARRAYSIZE(m_textureName));
		if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive())
		{
			ImGui::SetKeyboardFocusHere(1);
		}

		ImGui::MenuItem("Is the Model Righthanded?", "", &m_righthanded);

		ImGui::InputFloat3("Position", m_openPos);
		ImGui::InputFloat3("Rotation", m_openRot);
		ImGui::InputFloat3("Scale", m_openScale);

		if (ImGui::Button("Open"))
		{
			if (m_hasTexture)
			{
				pMeshComponent->SetMaterial(pGame->GetMaterial("Arcade_Cabinet"));

				pGame->GetMaterial("Arcade_Cabinet")->GetTexture()->SetTexture(m_textureName);
			}
			else
			{
				pMeshComponent->SetMaterial(pGame->GetMaterial("SolidColor"));
			}

			m_Objects[0]->SetPosition(vec3(m_openPos[0], m_openPos[1], m_openPos[2]));
			m_Objects[0]->SetRotation(vec3(m_openRot[0], m_openRot[1], m_openRot[2]));
			m_Objects[0]->SetScale(vec3(m_openScale[0], m_openScale[1], m_openScale[2]));

			pGame->GetMesh("Obj")->LoadObj(m_filename, m_righthanded);

			m_showOpenObj = false;
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
					pGame->ResetBackgroundColor(true);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Open", "Ctrl+O", &m_showOpenObj);
			
			if (ImGui::BeginMenu("Open Recent"))
			{
				m_Objects[0]->SetPosition(centerOfScreen + vec3(0.f, -3.5f, 0.f));
				m_Objects[0]->SetRotation(vec3());
				m_Objects[0]->SetScale(vec3(1.f, 1.f, 1.f));

				if(ImGui::MenuItem("Arcade_Cabinet.obj"))
				{
					pGame->GetMesh("Obj")->LoadObj("Data/Models/Arcade_Cabinet.obj", true);
					pMeshComponent->SetMaterial(pGame->GetMaterial("Arcade_Cabinet"));
					pGame->GetMaterial("Arcade_Cabinet")->GetTexture()->SetTexture("Data/Textures/Arcade_Cabinet.png");
				}
				if(ImGui::MenuItem("Chibi_Facehugger.obj"))
				{
					pGame->GetMesh("Obj")->LoadObj("Data/Models/Chibi_Facehugger.obj", true);
					pMeshComponent->SetMaterial(pGame->GetMaterial("SolidColor"));
				}
				if(ImGui::MenuItem("cube.obj"))
				{
					pGame->GetMesh("Obj")->LoadObj("Data/Models/cube.obj", true);
					pMeshComponent->SetMaterial(pGame->GetMaterial("SolidColor"));
				}
				if(ImGui::MenuItem(m_lastObj))
				{
					if (m_textureName != nullptr)
					{
						pMeshComponent->SetMaterial(pGame->GetMaterial("Arcade_Cabinet"));
						pGame->GetMaterial("Arcade_Cabinet")->GetTexture()->SetTexture(m_lastTexture);
					}
					else
					{
						pMeshComponent->SetMaterial(pGame->GetMaterial("SolidColor"));
					}
					pGame->GetMesh("Obj")->LoadObj(m_lastObj, true);
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Auto Rotation", "", &m_autoRotate)) {}
			ImGui::InputInt("Speed", &m_rotationSpeed, 1, 5);

			ImGui::MenuItem("Show Rotation Slider", "", &m_showSlider);

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Obj Loader", NULL, false, false);
		ImGui::EndMainMenuBar();
	}

    if (m_autoRotate)
    {
        offset = m_rotationSpeed * deltaTime;
    }

    m_Objects[0]->SetRotation(vec3(0.f, rot + offset, 0.f));
    m_Objects[1]->SetRotation(vec3(-90.f, rot + offset, 0.f));

}

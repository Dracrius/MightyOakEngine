#include "Framework.h"

#include "ThirdPersonScene.h"
#include "DataTypes.h"
#include "Game.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/SimplePlayer.h"
#include "DefaultSettings.h"

ThirdPersonScene::ThirdPersonScene(Game* pGame) : fw::Scene(pGame)
{
    m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
    m_pPhysicsWorld->SetGravity(c_gravity);

	m_pPlayerController = new PlayerController(pGame->GetFramework()->GetEventManager());

    vec3 cameraOffset = vec3(0.f, 0.f, -12.f);
	float heightOffset = -3.5f;

	m_openPos[0] = c_centerOfScreen.x;
	m_openPos[1] = c_centerOfScreen.y + heightOffset;
	m_openPos[2] = c_centerOfScreen.z;

    m_pCamera = new fw::Camera(this, c_centerOfScreen + cameraOffset);
	m_pCamera->SetAspectRatio(c_aspectRatio);

	char filename[260] = "Data/Models/Arcade_Cabinet.obj";
	strcpy_s(m_filename, filename);
	char textureName[260] = "Data/Textures/Arcade_Cabinet.png";
	strcpy_s(m_textureName, textureName);

	m_lastObj = filename;
	m_lastTexture = textureName;

	vec3 pos = c_centerOfScreen + vec3(0.f, heightOffset, 0.f);
	vec3 rot = vec3(-90.f, 0.f, 0.f);

    fw::GameObject* pObj= new fw::GameObject(this, pos, vec3());
    pObj->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Obj"), m_pResourceManager->GetMaterial("Arcade_Cabinet")));
	pObj->SetName("Loaded Obj");
    m_Objects.push_back(pObj);

    m_pResourceManager->GetMesh("Obj")->LoadObj(m_lastObj.c_str(), true);

	fw::GameObject* pCube = new fw::GameObject(this, c_centerOfScreen + vec3(-10, 0, 1), vec3());
	pCube->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
	pCube->SetName("Numbered Cube");
	m_Objects.push_back(pCube);

	pCube = new fw::GameObject(this, c_centerOfScreen + vec3(4, 0, 2), vec3());
	pCube->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
	pCube->SetName("Numbered Cube");
	m_Objects.push_back(pCube);

	pCube = new fw::GameObject(this, c_centerOfScreen + vec3(6, 0, 3), vec3());
	pCube->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
	pCube->SetName("Numbered Cube");
	m_Objects.push_back(pCube);

	SimplePlayer* pPlayer = new SimplePlayer(this, m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube"), c_centerOfScreen + vec3(-6, 0, -3), m_pPlayerController);
	pPlayer->SetName("Player");
	m_Objects.push_back(pPlayer);
	
	m_pCamera->AttachTo(pPlayer);
	//m_pCamera->SetThirdPersonOffset(vec3(-6, 3, -3));

    fw::GameObject* pFloor = new fw::GameObject(this, pos, rot);
    pFloor->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("Arcade_Floor")));
    pFloor->SetScale(vec3(28.f));
	pFloor->SetName("Floor");
    m_Objects.push_back(pFloor);
}

ThirdPersonScene::~ThirdPersonScene()
{
}

void ThirdPersonScene::StartFrame(float deltaTime)
{
}

void ThirdPersonScene::OnEvent(fw::Event* pEvent)
{
	fw::Scene::OnEvent(pEvent);
}

void ThirdPersonScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

	fw::FWCore* pFramework = static_cast<Game*>(m_pGame)->GetFramework();
	m_pCamera->Hack_ThirdPersonCam(pFramework, deltaTime);

	float rot = m_Objects[0]->GetRotation().y;
	float offset = 0.f;

	SettingsMenu();

	if (m_showSlider)
	{
		Slider(rot);
	}

	if (m_showOpenObj)
	{
		OpenObj();
	}
	else
	{
		m_openPos[0] = m_Objects[0]->GetPosition().x;
		m_openPos[1] = m_Objects[0]->GetPosition().y;
		m_openPos[2] = m_Objects[0]->GetPosition().z;
		m_openRot[0] = m_Objects[0]->GetRotation().x;
		m_openRot[1] = m_Objects[0]->GetRotation().y;
		m_openRot[2] = m_Objects[0]->GetRotation().z;
		m_openScale[0] = m_Objects[0]->GetScale().x;
		m_openScale[1] = m_Objects[0]->GetScale().y;
		m_openScale[2] = m_Objects[0]->GetScale().z;
	}
	
    if (m_autoRotate)
    {
        offset = m_rotationSpeed * deltaTime;
    }

    m_Objects[0]->SetRotation(vec3(m_Objects[0]->GetRotation().x, rot + offset, m_Objects[0]->GetRotation().z));
    m_Objects.back()->SetRotation(vec3(-90.f, rot + offset, 0.f));

}

void ThirdPersonScene::Slider(float& rot)
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

void ThirdPersonScene::SettingsMenu()
{
	Game* pGame = static_cast<Game*>(m_pGame);
	fw::MeshComponent* pMeshComponent = m_Objects[0]->GetComponent<fw::MeshComponent>();

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

			OpenRecent();

			ImGui::Separator();
			if (ImGui::MenuItem("Auto Rotation", "", &m_autoRotate)) {}
			ImGui::InputInt("Speed", &m_rotationSpeed, 1, 5);

			ImGui::MenuItem("Show Rotation Slider", "", &m_showSlider);

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Obj Loader", NULL, false, false);
		ImGui::EndMainMenuBar();
	}
}

void ThirdPersonScene::OpenObj()
{
	Game* pGame = static_cast<Game*>(m_pGame);
	fw::MeshComponent* pMeshComponent = m_Objects[0]->GetComponent<fw::MeshComponent>();

	ImGui::SetNextWindowSize(ImVec2(410, 210), ImGuiCond_Always);
	if (!ImGui::Begin("Open Obj", &m_showOpenObj))
	{
		ImGui::End();
		return;
	}

	bool autoRotateWas = m_autoRotate;
	m_autoRotate = false;

	m_lastTexture = m_textureName;
	m_lastObj = m_filename;

	ImGui::InputTextWithHint("Filename", "Data/Models/filename.obj", m_filename, IM_ARRAYSIZE(m_filename));
	ImGui::MenuItem("Does it have a Texture?", "", &m_hasTexture);
	ImGui::InputTextWithHint("Texture", "Data/Textures/filename.ext", m_textureName, IM_ARRAYSIZE(m_textureName));

	ImGui::MenuItem("Is the Model Righthanded?", "", &m_righthanded);

	ImGui::InputFloat3("Position", m_openPos);
	ImGui::InputFloat3("Rotation", m_openRot);
	ImGui::InputFloat3("Scale", m_openScale);

	if (ImGui::Button("Open"))
	{
		if (m_hasTexture)
		{
			pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("Arcade_Cabinet"));

			m_pResourceManager->GetMaterial("Arcade_Cabinet")->GetTexture()->SetTexture(m_textureName);
		}
		else
		{
			pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("SolidColor"));
			m_lastTexture = "";
		}

		m_Objects[0]->SetPosition(vec3(m_openPos[0], m_openPos[1], m_openPos[2]));
		m_Objects[0]->SetRotation(vec3(m_openRot[0], m_openRot[1], m_openRot[2]));
		m_Objects[0]->SetScale(vec3(m_openScale[0], m_openScale[1], m_openScale[2]));

		m_pResourceManager->GetMesh("Obj")->LoadObj(m_filename, m_righthanded);

		m_showOpenObj = false;
		m_autoRotate = autoRotateWas;
	}

	ImGui::End();
}

void ThirdPersonScene::OpenRecent()
{
	Game* pGame = static_cast<Game*>(m_pGame);
	fw::MeshComponent* pMeshComponent = m_Objects[0]->GetComponent<fw::MeshComponent>();

	if (ImGui::BeginMenu("Open Recent"))
	{
		float heightOffset = -3.5f;
		vec3 pos = c_centerOfScreen + vec3(0.f, heightOffset, 0.f);

		if (ImGui::MenuItem("Arcade_Cabinet.obj"))
		{
			m_Objects[0]->SetPosition(pos);
			m_Objects[0]->SetRotation(vec3());
			m_Objects[0]->SetScale(vec3(1.f, 1.f, 1.f));

			m_pResourceManager->GetMesh("Obj")->LoadObj("Data/Models/Arcade_Cabinet.obj", true);
			pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("Arcade_Cabinet"));
			m_pResourceManager->GetMaterial("Arcade_Cabinet")->GetTexture()->SetTexture("Data/Textures/Arcade_Cabinet.png");
		}
		if (ImGui::MenuItem("Chibi_Facehugger.obj"))
		{
			m_Objects[0]->SetPosition(pos);
			m_Objects[0]->SetRotation(vec3(-90.f, 0.f, 0.f));
			m_Objects[0]->SetScale(vec3(0.1f, 0.1f, 0.1f));

			m_pResourceManager->GetMesh("Obj")->LoadObj("Data/Models/Chibi_Facehugger.obj", true);
			pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("SolidColor"));
		}
		if (ImGui::MenuItem("cube.obj"))
		{
			m_Objects[0]->SetPosition(c_centerOfScreen + vec3(0.f, -2.5f, 0.f));
			m_Objects[0]->SetRotation(vec3());
			m_Objects[0]->SetScale(vec3(1.f, 1.f, 1.f));

			m_pResourceManager->GetMesh("Obj")->LoadObj("Data/Models/cube.obj", true);
			pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("SolidColor"));
		}
		if (ImGui::MenuItem(m_lastObj.c_str()))
		{
			if (m_textureName != nullptr && m_lastTexture != "")
			{
				pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("Arcade_Cabinet"));
				m_pResourceManager->GetMaterial("Arcade_Cabinet")->GetTexture()->SetTexture(m_lastTexture.c_str());
			}
			else
			{
				pMeshComponent->SetMaterial(m_pResourceManager->GetMaterial("SolidColor"));
			}

			m_Objects[0]->SetPosition(pos);
			m_Objects[0]->SetRotation(vec3());
			m_Objects[0]->SetScale(vec3(1.f, 1.f, 1.f));

			m_pResourceManager->GetMesh("Obj")->LoadObj(m_lastObj.c_str(), true);
		}
		ImGui::EndMenu();
	}
}


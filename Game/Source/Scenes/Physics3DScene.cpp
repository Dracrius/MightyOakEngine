#include "Framework.h"

#include "Physics3DScene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Player3D.h"
#include "Game.h"
#include "DefaultSettings.h"

Physics3DScene::Physics3DScene(Game* pGame) : fw::Scene(pGame)
{
	m_pPhysicsWorld = new fw::PhysicsWorldBullet(pGame->GetFramework()->GetEventManager());
	m_pPhysicsWorld->SetGravity(c_gravity);

	m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);
	m_pCamera->SetPerspective(true);

	m_pPlayerController = new PlayerController(pGame->GetFramework()->GetEventManager());

	vec3 pos = c_centerOfScreen + vec3(1.5f, 1.f, 2.f);
	vec3 rot = vec3(90.f, 0.f, 0.f);

	fw::GameObject* pBackground = new fw::GameObject(this, pos + vec3(0.f, 0.f, 10.f), rot);
	pBackground->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Background"), m_pResourceManager->GetMaterial("Background")));
	pBackground->SetScale(vec3(18.8f, 0.f, 10.f));
	pBackground->SetName("Background");
	m_Objects.push_back(pBackground);

	for (int i = 0; i < 6; i++)
	{
		vec3 pos = c_centerOfScreen;
		if (i < 3)
		{
			pos += vec3(1.f - (float)i, 0.f, 0.f);
		}
		else if (i >= 3 && i < 5)
		{
			pos += vec3(3.5f - (float)i, 1.f, 0.f);
		}
		else
		{
			pos += vec3(0.f, 2.f, 0.f);
		}

		std::string name = "Numbered Box " + i;
		fw::GameObject* pBox = new fw::GameObject(this, pos, vec3());
		pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube")));
		pBox->CreateBody(m_pPhysicsWorld, true, vec3(1.0f, 1.0f, 1.0f), 1.f);
		pBox->SetName(name);
		m_Objects.push_back(pBox);
	}



	fw::GameObject* pPlatform = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -4.5f, 0.f), vec3(0.f, 0.f, 0.f));

	fw::MeshComponent* pPlatformMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("SolidColor"));

	pPlatform->AddComponent(pPlatformMesh);
	pPlatform->SetScale(vec3(20.f, 1.f, 10.f));
	pPlatform->CreateBody(m_pPhysicsWorld, false, vec3(20.0f, 1.0f, 10.0f), 0.f);
	pPlatform->SetName("Platform");
	m_Objects.push_back(pPlatform);

	Player3D* pPlayer = new Player3D(this, m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Cube"), vec2(7.5f, 16.0f), m_pPlayerController);
	pPlayer->CreateBody(m_pPhysicsWorld, true, vec3(1.f), 1.f);
	pPlayer->SetName("Player");
	m_Objects.push_back(pPlayer);

	m_pCamera->SetAspectRatio(c_aspectRatio);
    m_pCamera->AttachTo(pPlayer);
}

Physics3DScene::~Physics3DScene()
{
    delete m_pPlayerController;
}

void Physics3DScene::StartFrame(float deltaTime)
{
    m_pPlayerController->StartFrame();
}

void Physics3DScene::OnEvent(fw::Event* pEvent)
{
    fw::Scene::OnEvent(pEvent);
}

void Physics3DScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

    fw::FWCore* pFramework = static_cast<Game*>(m_pGame)->GetFramework();
    m_pCamera->Hack_ThirdPersonCam(pFramework, deltaTime);

	ControlsMenu();

	//Figure out world space coordinate of mosue in Ortho.
	//{
	//	ivec2 mouse; //Window Coodinates
	//	
	//	Game* pGame = static_cast<Game*>(m_pGame);

	//	pGame->GetFramework()->GetMouseCoordinates(&mouse.x, &mouse.y);

	//	ivec2 viewPortMouse = mouse;
	//	ivec2 windowSize(pGame->GetFramework()->GetWindowWidth(), pGame->GetFramework()->GetWindowHeight());

	//	//Window to OpenGL Render Space ie View Port

	//	//Flip Window Coord
	//	viewPortMouse.x = viewPortMouse.x;
	//	viewPortMouse.y = windowSize.y - viewPortMouse.y;

	//	//Find View Coord
	//	viewPortMouse.x -= (windowSize.x - c_glRenderSize.x) / 2;
	//	viewPortMouse.y -= (windowSize.y - c_glRenderSize.y) / 2;

	//	//Find Clip Space Coord
	//	vec2 clipSpaceMouse = viewPortMouse / (c_glRenderSize / 2.f) - 1.f;

	//	//Find View Space Coord
	//	fw::matrix invProj;
	//	invProj = m_pCamera->GetProjecMatrix().GetInverse();

	//	vec2 viewSpaceMouse = invProj * clipSpaceMouse;

	//	//Find World Space Coord
	//	fw::matrix invView = m_pCamera->GetViewMatrix().GetInverse();
	//	vec2 worldSpaceMouse = invView * viewSpaceMouse;


	//	ImGui::Text("Mouse Window Coords: %d, %d", mouse.x, mouse.y);
	//	ImGui::Text("Mouse View Port Coords: %d, %d", viewPortMouse.x, viewPortMouse.y);
	//	ImGui::Text("Mouse Clip Space Coords: %0.2f, %0.2f", clipSpaceMouse.x, clipSpaceMouse.y);
	//	ImGui::Text("Mouse View Space Coords: %0.2f, %0.2f", viewSpaceMouse.x, viewSpaceMouse.y);
	//	ImGui::Text("Mouse World Space Coords: %0.2f, %0.2f", worldSpaceMouse.x, worldSpaceMouse.y);

	//}
	//Figure out world space coordinate of mosue in Perspective.
	{
		ivec2 mouse; //Window Coodinates

		Game* pGame = static_cast<Game*>(m_pGame);

		pGame->GetFramework()->GetMouseCoordinates(&mouse.x, &mouse.y);

		ivec2 viewPortMouse = mouse;
		ivec2 windowSize(pGame->GetFramework()->GetWindowWidth(), pGame->GetFramework()->GetWindowHeight());

		//Window to OpenGL Render Space ie View Port

		//Flip Window Coord
		viewPortMouse.x = viewPortMouse.x;
		viewPortMouse.y = windowSize.y - viewPortMouse.y;

		//Find View Coord
		viewPortMouse.x -= (windowSize.x - c_glRenderSize.x) / 2;
		viewPortMouse.y -= (windowSize.y - c_glRenderSize.y) / 2;

		//Find Clip Space Coord
		vec2 clipSpaceMouse = viewPortMouse / (c_glRenderSize / 2.f) - 1.f;
		vec4 clipSpaceMouse4 = vec4(clipSpaceMouse, 1, 1);

		//Find View Space Coord
		fw::matrix invProj = m_pCamera->GetProjecMatrix().GetInverse();
		vec4 viewSpaceMouse4 = invProj * clipSpaceMouse4;

		//Find World Space Coord
		fw::matrix invView = m_pCamera->GetViewMatrix().GetInverse();
		vec4 worldSpaceMouse4 = invView * viewSpaceMouse4;

		vec3 nearPosition = m_pCamera->GetPosition();
		vec3 farPosition = worldSpaceMouse4.XYZ() / worldSpaceMouse4.w;

		float zDesired = 0.f;
		vec3 rayDir = farPosition - nearPosition;
		float rayPerc = (zDesired - nearPosition.z) / rayDir.z;
		vec3 zDesiredMouse = nearPosition + rayDir * rayPerc;


		ImGui::Text("Mouse Window Coords: %d, %d", mouse.x, mouse.y);
		ImGui::Text("Mouse View Port Coords: %d, %d", viewPortMouse.x, viewPortMouse.y);
		ImGui::Text("Mouse Clip Space Coords: %0.2f, %0.2f", clipSpaceMouse.x, clipSpaceMouse.y);
		ImGui::Text("Mouse View Space Coords: %0.2f, %0.2f", viewSpaceMouse4.x, viewSpaceMouse4.y);
		ImGui::Text("Mouse World Space Coords: %0.2f, %0.2f, %0.2f", zDesiredMouse.x, zDesiredMouse.y, zDesiredMouse.z);

	}
}

void Physics3DScene::ControlsMenu()
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

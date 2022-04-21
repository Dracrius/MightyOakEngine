#include "Framework.h"
#include "DefaultSettings.h"

#include "Physics3DScene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Player3D.h"
#include "Game.h"
#include "Components/Player3DMovementComponent.h"

Physics3DScene::Physics3DScene(Game* pGame) : fw::Scene(pGame)
{
    pGame->GetFramework()->GetEventManager()->RegisterForEvents(fw::CollisionEvent::GetStaticEventType(), this);

	m_pPhysicsWorld = new fw::PhysicsWorldBullet(pGame->GetFramework()->GetEventManager());
	m_pPhysicsWorld->SetGravity(c_gravity);

    vec3 cameraOffset = vec3(0.f, 1.f, 0.f);

	m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);
	m_pCamera->SetPerspective(true);
    m_pCamera->SetAspectRatio(c_aspectRatio);
    m_pCamera->SetThirdPersonOffset(cameraOffset);
    m_pCamera->SetRotation(vec3(-10.f, 0.f, 0.f));

	m_pPlayerController = new PlayerController(pGame->GetFramework()->GetEventManager());

	vec3 pos = c_centerOfScreen + vec3(1.5f, 1.f, 2.f);
	vec3 rot = vec3(90.f, 0.f, 0.f);

    //Load 6 Boxes
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

		std::string name = "Numbered Box " + std::to_string(i + 1);
		fw::GameObject* pBox = new fw::GameObject(this, pos, vec3());
		pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Lit-Cube")));
        pBox->AddComponent(new fw::PhysicsBodyComponent());
        pBox->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, true, vec3(1.0f, 1.0f, 1.0f), 1.f);
		pBox->SetName(name);
		m_Objects.push_back(pBox);
	}

	//p2p Joint
	std::string name = "p2p Joint Box";
	fw::GameObject* pBox = new fw::GameObject(this, vec3(12.f, 4.5f, 3.f), vec3());
	pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Swing")));
	pBox->AddComponent(new fw::PhysicsBodyComponent());
	pBox->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, true, vec3(1.0f, 1.0f, 1.0f), 1.f);
	pBox->SetName(name);
	m_Objects.push_back(pBox);

	m_pPhysicsWorld->CreateJoint(pBox->GetComponent<fw::PhysicsBodyComponent>()->GetPhysicsBody(), vec3(0.f, 10.0f, 0.f));

	//Slider
	name = "Slider Box";
	pBox = new fw::GameObject(this, vec3(2.f, 3.75f, -3.f), vec3());
	pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Slide")));
	pBox->AddComponent(new fw::PhysicsBodyComponent());
	pBox->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, true, vec3(1.0f, 1.0f, 1.0f), 1.f);
	pBox->SetName(name);
	m_Objects.push_back(pBox);

	m_pPhysicsWorld->CreateSlider(pBox->GetComponent<fw::PhysicsBodyComponent>()->GetPhysicsBody(), vec3(2.f, 0.0f, 0.f));

    //Buttons
    name = "On Button";
    pBox = new fw::GameObject(this, vec3(1.f, 3.25f, 3.f), vec3());
    pBox->SetScale(vec3(1.f, 0.25f, 1.f));
    pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("On")));
    pBox->SetName(name);
    m_Objects.push_back(pBox);

    m_pPhysicsWorld->CreateSensor(pBox, pBox->GetTransform(), true);

    name = "Off Button";
    pBox = new fw::GameObject(this, vec3(3.f, 3.25f, 3.f), vec3());
    pBox->SetScale(vec3(1.f, 0.25f, 1.f));
    pBox->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Off")));
    pBox->SetName(name);
    m_Objects.push_back(pBox);

    m_pPhysicsWorld->CreateSensor(pBox, pBox->GetTransform(), true);

    //Lights
    fw::GameObject* pLight = new fw::GameObject(this, vec3(2.f, 10.f, 3.f), vec3(-90.f, 0.f, 0.f));
    pLight->AddComponent(new fw::LightComponent(fw::LightType::SpotLight, Color4f(0.f, 0.f, 0.f, 1.f), 20.f, 2.f, 60.f));
    pLight->SetName("Spot Light");
    m_Objects.push_back(pLight);

    pLight = new fw::GameObject(this, vec3(-9.f, 10.f, -3.f), vec3(-30.f, -90.f, 0.f));
    pLight->AddComponent(new fw::LightComponent(fw::LightType::SpotLight, Color4f(2.f, 2.f, 2.f, 1.f), 40.f, 2.f, 20.f));
    pLight->SetName("Slider Light");
    m_Objects.push_back(pLight);

    pLight = new fw::GameObject(this, vec3(9.f, 10.f, -3.f), vec3());
    pLight->AddComponent(new fw::LightComponent(fw::LightType::PointLight, Color4f(0.2f, 0.f, 0.3f, 1.f), 40.f, 2.f, 20.f));
    pLight->SetName("Area Light");
    m_Objects.push_back(pLight);

    pLight = new fw::GameObject(this, vec3(12.f, 10.f, 3.f), vec3(-90.f, 0.f, 0.f));
    pLight->AddComponent(new fw::LightComponent(fw::LightType::SpotLight, Color4f(1.f, 1.f, 1.f, 1.f), 20.f, 2.f, 60.f));
    pLight->SetName("Joint Light");
    m_Objects.push_back(pLight);

    pLight = new fw::GameObject(this, c_centerOfScreen + vec3(-7.f, 10.f, -7.f), vec3(-45.f, 45.f, 0.f));
    pLight->AddComponent(new fw::LightComponent(fw::LightType::Directional, Color4f(0.5f, 0.5f, 0.5f, 1.f), 10.f, 2.f));
    pLight->SetName("Directional Light");
    m_Objects.push_back(pLight);

    //Platform
	fw::GameObject* pPlatform = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -4.5f, 0.f), vec3(0.f, 0.f, 0.f));

	fw::MeshComponent* pPlatformMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Lit-DarkPurple"));

	pPlatform->AddComponent(pPlatformMesh);
	pPlatform->SetScale(vec3(25.f, 0.5f, 15.f));
    pPlatform->AddComponent(new fw::PhysicsBodyComponent());
	pPlatform->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, false, vec3(25.0f, 0.5f, 15.0f), 0.f);
	pPlatform->SetName("Platform");
	m_Objects.push_back(pPlatform);

    fw::GameObject* pWall = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -4.5f, 8.f), vec3(0.f, 0.f, 0.f));
    pWall->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Lit-DarkPurple")));
    pWall->SetScale(vec3(25.0f, 1.f, 1.f));
    pWall->AddComponent(new fw::PhysicsBodyComponent());
    pWall->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, false, vec3(25.0f, 2.f, 1.0f), 0.f);
    pWall->SetName("Back Wall");
    m_Objects.push_back(pWall);

    pWall = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -4.5f, -8.f), vec3(0.f, 0.f, 0.f));
    pWall->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Lit-DarkPurple")));
    pWall->SetScale(vec3(25.0f, 1.f, 1.f));
    pWall->AddComponent(new fw::PhysicsBodyComponent());
    pWall->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, false, vec3(25.0f, 2.f, 1.0f), 0.f);
    pWall->SetName("Front Wall");
    m_Objects.push_back(pWall);

    pWall = new fw::GameObject(this, c_centerOfScreen + vec3(-13.f, -4.5f, 0.f), vec3(0.f, 0.f, 0.f));
    pWall->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Lit-DarkPurple")));
    pWall->SetScale(vec3(1.f, 1.f, 17.f));
    pWall->AddComponent(new fw::PhysicsBodyComponent());
    pWall->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, false, vec3(1.f, 2.f, 17.f), 0.f);
    pWall->SetName("Left Wall");
    m_Objects.push_back(pWall);

    pWall = new fw::GameObject(this, c_centerOfScreen + vec3(13.f, -4.5f, 0.f), vec3(0.f, 0.f, 0.f));
    pWall->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Cube"), m_pResourceManager->GetMaterial("Lit-DarkPurple")));
    pWall->SetScale(vec3(1.f, 1.f, 17.f));
    pWall->AddComponent(new fw::PhysicsBodyComponent());
    pWall->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, false, vec3(1.f, 2.f, 17.f), 0.f);
    pWall->SetName("Right Wall");
    m_Objects.push_back(pWall);

    //Player
    m_pPlayer = new fw::GameObject(this, vec2(7.5f, 16.0f), vec3());
    m_pPlayer->SetScale(vec3(0.5f, 0.5f, 0.5f));
    m_pPlayer->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Sphere"), m_pResourceManager->GetMaterial("Lit-SolidColor")));
    m_pPlayer->AddComponent(new fw::PhysicsBodyComponent());
    m_pPlayer->GetComponent<fw::PhysicsBodyComponent>()->CreateBody(m_pPhysicsWorld, true, 0.5f, 1.f);
    m_pPlayer->AddComponent(new Player3DMovementComponent(m_pPlayerController));
    m_pPlayer->SetName("Player");

    m_pCamera->AttachTo(m_pPlayer);
}

Physics3DScene::~Physics3DScene()
{
    delete m_pPlayerController;

    delete m_pPlayer;
}

void Physics3DScene::StartFrame(float deltaTime)
{
    m_pPlayerController->StartFrame();
}

void Physics3DScene::OnEvent(fw::Event* pEvent)
{
    fw::LightComponent* pLight = nullptr;

    if (pEvent->GetEventType() == "CollisionEvent")
    {
        fw::CollisionEvent* pCollisionEvent = static_cast<fw::CollisionEvent*>(pEvent);

        fw::GameObject* collisObjOne = pCollisionEvent->GetGameObjectOne();
        fw::GameObject* collisObjTwo = pCollisionEvent->GetGameObjectTwo();

        if (collisObjTwo->GetName() == "Player" && collisObjOne->GetName() == "On Button")
        {
            for (size_t i = 0; i < m_Objects.size(); i++)
            {
                if (m_Objects[i]->GetName() == "Spot Light")
                {
                    pLight = m_Objects[i]->GetComponent<fw::LightComponent>();

                    pLight->SetDiffuse(Color4f::White());
                }
            }
        }
        else if (collisObjTwo->GetName() == "Player" && collisObjOne->GetName() == "Off Button")
        {
            for (size_t i = 0; i < m_Objects.size(); i++)
            {
                if (m_Objects[i]->GetName() == "Spot Light")
                {
                    pLight = m_Objects[i]->GetComponent<fw::LightComponent>();

                    pLight->SetDiffuse(Color4f::Black());
                }
            }
        }
    }

    fw::Scene::OnEvent(pEvent);
}

void Physics3DScene::Update(float deltaTime)
{
    static_cast<Game*>(m_pGame)->SetUsingCubeMap(true);
    static_cast<Game*>(m_pGame)->SetCurrentCubeMap("NightMeadow");

    std::vector<fw::Component*>& list = m_pComponentManager->GetComponentsOfType(Player3DMovementComponent::GetStaticType());
    for (fw::Component* pComponent : list)
    {
        Player3DMovementComponent* pPlayerComp = static_cast<Player3DMovementComponent*>(pComponent);
        pPlayerComp->Update(deltaTime);
    }

    Scene::Update(deltaTime);

    fw::FWCore* pFramework = static_cast<Game*>(m_pGame)->GetFramework();
    m_pCamera->Hack_ThirdPersonCam(pFramework, deltaTime);

	ControlsMenu();

    if (m_showPerspecMouseCoords)
    {
        ShowPerspecMouseCoordinates();
    }
}

void Physics3DScene::ShowPerspecMouseCoordinates()
{
    ImGui::SetNextWindowSize(ImVec2(325, 120), ImGuiCond_Always);
    if (!ImGui::Begin("Perspective Mouse Coords", &m_showPerspecMouseCoords))
    {
        ImGui::End();
        return;
    }

    //Figure out world space coordinate of mosue in Perspective.
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

    ImGui::End();
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
            //if (ImGui::MenuItem("Show Perspective Mouse Coords", "", &m_showPerspecMouseCoords)) {};

            //ImGui::Separator();

			if (ImGui::MenuItem("Enable Debug Draw", NULL, false, false)) {};//&m_debugDraw)) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Controls"))
		{
			ImGui::Text("Forward          = W");
			ImGui::Text("Backward         = S");
            ImGui::Text("Strafe Left      = A");
            ImGui::Text("Strafe Right     = D");
            ImGui::Separator();
            ImGui::Text("Camera Tilt Up   = I  or  Up");
            ImGui::Text("Camera Tilt Down = K  or  Down");
			ImGui::Text("Camera Pan Left  = J  or  Left");
			ImGui::Text("Camera Pan Right = L  or  Right");

			ImGui::EndMenu();
		}
		ImGui::MenuItem("Assignment 2 Scene - 3D Physics Playground", NULL, false, false);

		ImGui::EndMainMenuBar();
	}
}

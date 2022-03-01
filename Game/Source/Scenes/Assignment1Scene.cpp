#include "Framework.h"

#include "Assignment1Scene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Shaun.h"
#include "GameObjects/Meteor.h"
#include "Game.h"
#include "DefaultSettings.h"

Assignment1Scene::Assignment1Scene(Game* pGame) : fw::Scene(pGame)
{
	m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
	m_pPhysicsWorld->SetGravity(c_gravity);

	m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);

	m_pPlayerController = new PlayerController();

	vec3 pos = c_centerOfScreen + vec3(1.5f, 1.f, 2.f);
	vec3 rot = vec3(90.f, 0.f, 0.f);

	fw::GameObject* pBackground = new fw::GameObject(this, pos, rot);
	pBackground->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Background"), m_pResourceManager->GetMaterial("Background")));
	pBackground->SetScale(vec3(18.8f, 0.f, 10.f));
	pBackground->SetName("Background");
	m_Objects.push_back(pBackground);

	//Platform
	{
		fw::GameObject* pPlatform = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -4.5f, 0.f), vec3(0.f, 0.f, 0.f));

		fw::MeshComponent* pPlatformMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Platform"), m_pResourceManager->GetMaterial("PlatformCenter"));

		//pPlatformMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_02")->uvScale);
		//pPlatformMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_02")->uvOffset);

		pPlatform->AddComponent(pPlatformMesh);
		pPlatform->SetScale(vec3(20.f, 2.f, 0.f));
		pPlatform->CreateBody(m_pPhysicsWorld, false, vec3(20.0f, 2.0f, 2.0f), 1.f);
		pPlatform->SetName("Platform");
		m_Objects.push_back(pPlatform);

		fw::GameObject* pLeftEdge = new fw::GameObject(this, c_centerOfScreen + vec3(-10.9f, -4.5f, 0.f), vec3());

		fw::MeshComponent* pLeftEdgeMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));

		pLeftEdgeMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_01")->uvScale);
		pLeftEdgeMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_01")->uvOffset);
		pLeftEdge->AddComponent(pLeftEdgeMesh);
		pLeftEdge->SetScale(vec3(2.f, 2.f, 0.f));
		pLeftEdge->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
		pLeftEdge->SetName("Platform");
		m_Objects.push_back(pLeftEdge);

		fw::GameObject* pRightEdge = new fw::GameObject(this, c_centerOfScreen + vec3(10.9f, -4.5f, 0.f), vec3());

		fw::MeshComponent* pRightEdgeMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));

		pRightEdgeMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_03")->uvScale);
		pRightEdgeMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_03")->uvOffset);
		pRightEdge->AddComponent(pRightEdgeMesh);
		pRightEdge->SetScale(vec3(2.f, 2.f, 0.f));
		pRightEdge->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
		pRightEdge->SetName("Platform");
		m_Objects.push_back(pRightEdge);
	}

	//Meteors
	{
		Meteor* pMeteor = new Meteor(this, m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"), m_pResourceManager->GetSpriteSheet("NiceDaysWalk"), vec2(c_centerOfScreen.x + 5.f, c_centerOfScreen.y), vec3());
		pMeteor->CreateBody(m_pPhysicsWorld, true, 1.f);
		pMeteor->SetName("Meteor");
		m_Objects.push_back(pMeteor);
	}

	//Debris
	{
		for (int i = 0; i < 9; i++)
		{
			fw::GameObject* pDebris = new fw::GameObject(this, vec2(c_centerOfScreen.x - 5.f, c_centerOfScreen.y), vec3());
			fw::MeshComponent* pDebrisMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));
			pDebris->CreateBody(m_pPhysicsWorld, true, 0.4f, 1.f);

			if (i < 2)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Lamb_Chop_01")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Lamb_Chop_01")->uvOffset);

				if (i == 0)
				{
					pDebris->ApplyImpulse(vec3(0.81f, 0.58f, 0.f) * 4);
				}
				if (i == 1)
				{
					pDebris->ApplyImpulse(vec3(-0.81f, 0.58f, 0.f) * 4);
				}
			}
			else if (i < 4)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Bone_01")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Bone_01")->uvOffset);

				if (i == 2)
				{
					pDebris->ApplyImpulse(vec3(0.58f, 0.81f, 0.f) * 4);
				}
				if (i == 3)
				{
					pDebris->ApplyImpulse(vec3(-0.58f, 0.81f, 0.f) * 4);
				}
			}
			else if (i == 4)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_01")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_01")->uvOffset);
				pDebris->ApplyImpulse(vec3(0.95f, 0.3f, 0.f) * 4);
			}
			else if (i == 5)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_02")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_02")->uvOffset);
				pDebris->ApplyImpulse(vec3(0.3f, 0.95f, 0.f) * 4);
			}
			else if (i == 6)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_03")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_03")->uvOffset);
				pDebris->ApplyImpulse(vec3(-0.95f, 0.3f, 0.f) * 4);
			}
			else if (i == 7)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_04")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Leg_04")->uvOffset);
				pDebris->ApplyImpulse(vec3(-0.3f, 0.95f, 0.f) * 4);
			}
			else if (i == 8)
			{
				pDebrisMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Wool_Chunck_01")->uvScale);
				pDebrisMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Wool_Chunck_01")->uvOffset);
				pDebris->ApplyImpulse(vec3(0.f, 1.f, 0.f) * 4);
			}

			pDebris->AddComponent(pDebrisMesh);
			pDebris->SetName("Debris");
			m_Objects.push_back(pDebris);
		}
	}

	Shaun* pShaun = new Shaun(this, m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"), vec2(7.5f, 12.0f), m_pPlayerController);
	pShaun->SetSpriteSheet(m_pResourceManager->GetSpriteSheet("NiceDaysWalk"));
	pShaun->SetScale(vec3(2.f, 2.f, 0.f));
	pShaun->CreateBody(m_pPhysicsWorld, true, vec3(c_shaunCollider.x, c_shaunCollider.y, c_shaunCollider.y) * 2, 1.f);
	pShaun->SetName("Shaun the Sheep");
	m_Objects.push_back(pShaun);

	m_pCamera->AttachTo(m_Objects.back());
	m_pCamera->SetThirdPerson(c_cameraOffset + vec3(0.f, 4.5f, 0.f));
	m_pCamera->SetAspectRatio(c_aspectRatio);
}

Assignment1Scene::~Assignment1Scene()
{
    delete m_pPlayerController;
}

void Assignment1Scene::StartFrame(float deltaTime)
{
    m_pPlayerController->StartFrame();
}

void Assignment1Scene::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent(pEvent);

	if (pEvent->GetEventType() == "CollisionEvent")
	{
		fw::CollisionEvent* pCollisionEvent = static_cast<fw::CollisionEvent*>(pEvent);

		fw::ContactState contactState = pCollisionEvent->GetContactState();
		fw::GameObject* collisObjOne = pCollisionEvent->GetGameObjectOne();
		fw::GameObject* collisObjTwo = pCollisionEvent->GetGameObjectTwo();

		if ((collisObjOne->GetName() == "Shaun the Sheep" && collisObjTwo->GetName() == "Platform"))
		{
			static_cast<Shaun*>(collisObjOne)->SetIsOnGround(true);
		}
		if (collisObjOne->GetName() == "Platform" && collisObjTwo->GetName() == "Shaun the Sheep")
		{
			static_cast<Shaun*>(collisObjTwo)->SetIsOnGround(true);
		}
	}

    fw::Scene::OnEvent(pEvent);
}

void Assignment1Scene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

	ControlsMenu();
}

void Assignment1Scene::ControlsMenu()
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
		ImGui::MenuItem("Assignment 1 Scene", NULL, false, false);

		ImGui::EndMainMenuBar();
	}
}

#include "Framework.h"

#include "Assignment1Scene.h"
#include "DataTypes.h"
#include "GameObjects/PlayerController.h"
#include "GameObjects/Shaun.h"
#include "GameObjects/Meteor.h"
#include "Game.h"

Assignment1Scene::Assignment1Scene(Game* pGame) : fw::Scene(pGame)
{
	m_pPhysicsWorld = new fw::PhysicsWorldBox2D(pGame->GetFramework()->GetEventManager());
	m_pPhysicsWorld->SetGravity(c_gravity);

	m_pCamera = new fw::Camera(this, c_centerOfScreen + c_cameraOffset);

	m_pPlayerController = new PlayerController();

	vec3 pos = c_centerOfScreen + vec3(1.5f, 0.5f, 2.f);
	vec3 rot = vec3(90.f, 0.f, 0.f);

	fw::GameObject* pBackground = new fw::GameObject(this, pos, rot);
	pBackground->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Background"), m_pResourceManager->GetMaterial("Background")));
	pBackground->SetScale(vec3(18.8f, 0.f, 10.f));
	pBackground->SetName("Background");
	m_Objects.push_back(pBackground);

	SetupPlatform();

	FillMeteorPool();

	FillDebrisPool();

	fw::GameObject* pVictory = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -7.f, 0.f), vec3(0.f, 0.f, 0.f));
	pVictory->CreateBody(m_pPhysicsWorld, false, vec3(80.0f, 2.0f, 2.0f), 1.f);
	pVictory->SetName("Victory Box");
	m_Objects.push_back(pVictory);

	Shaun* pShaun = new Shaun(this, m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"), vec2(7.5f, 6.0f), m_pPlayerController);
	pShaun->SetSpriteSheet(m_pResourceManager->GetSpriteSheet("NiceDaysWalk"));
	pShaun->SetScale(vec3(2.f, 2.f, 0.f));
	pShaun->CreateBody(m_pPhysicsWorld, true, vec3(c_shaunCollider.x, c_shaunCollider.y, c_shaunCollider.y) * 2, 1.f);
	pShaun->SetName("Shaun the Sheep");
	m_Objects.push_back(pShaun);

	//m_pCamera->AttachTo(m_Objects.back());
	//m_pCamera->SetThirdPerson(c_cameraOffset + vec3(0.f, 4.5f, 0.f));
	m_pCamera->SetAspectRatio(c_aspectRatio);
}

Assignment1Scene::~Assignment1Scene()
{
    delete m_pPlayerController;

	for (fw::GameObject* pDebris : m_debris)
	{
		delete pDebris;
	}

	for (fw::GameObject* pMeteors : m_meteors)
	{
		delete pMeteors;
	}
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

		Shaun* pShaun = static_cast<Shaun*>(CheckCollision(pCollisionEvent, "Shaun the Sheep", "Platform"));
		if (pShaun)
		{
			pShaun->SetIsOnGround(true);
		}

		pShaun = static_cast<Shaun*>(CheckCollision(pCollisionEvent, "Shaun the Sheep", "Victory Box"));
		if (pShaun)
		{
			pShaun->SetState(false);
			m_showWin = true;
		}

		pShaun = static_cast<Shaun*>(CheckCollision(pCollisionEvent, "Shaun the Sheep", "Meteor"));
		if (pShaun)
		{
			pShaun->SetState(false);
			SpawnDebris(pShaun->GetPosition());
			m_showDeath = true;
		}

		fw::GameObject* pMeteor = CheckCollision(pCollisionEvent, "Meteor", "Shaun the Sheep");
		if (!pMeteor)
		{
			pMeteor = CheckCollision(pCollisionEvent, "Meteor", "Platform");
		}
		if (!pMeteor)
		{
			pMeteor = CheckCollision(pCollisionEvent, "Meteor", "Victory Box");
		}
		if (pMeteor)
		{
			pMeteor->SetState(false);
			m_pCamera->ShakeCamera();

			std::vector<fw::GameObject*>::iterator it;
			for (it = m_Objects.begin(); it != m_Objects.end();)
			{
				if ((*it) == pMeteor)
				{
					m_meteors.push_back(pMeteor);
					it = m_Objects.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		fw::GameObject* pDebris = CheckCollision(pCollisionEvent, "Debris", "Victory Box");
		if (pDebris)
		{
			pDebris->SetState(false);
		}

	}

    fw::Scene::OnEvent(pEvent);
}

void Assignment1Scene::Update(float deltaTime)
{
    Scene::Update(deltaTime);

	ControlsMenu();

	if (m_showStart)
	{
		StartGameWindow();
	}

	if (m_showWin)
	{
		WinWindow();
	}

	if (m_showDeath)
	{
		DeathWindow();
	}

	if (m_isPlaying)
	{
		m_meteorTimer -= deltaTime;

		if (m_meteorTimer <= 0.f)
		{
			//Spawn Meteor
			vec3 randDirect = vec3(fw::Random::GetFloat(-10.f, 10.f), -20.f, 0.f);
			vec3 randPos = vec3(fw::Random::GetFloat(-4.f, 20.f), 20.f, 0.f);
			vec3 randTorque = vec3(0.f, 0.f, fw::Random::GetFloat(100.f, 150.f));

			bool coinFlip = fw::Random::GetInt(0, 1);
			if (coinFlip)
			{
				randTorque = vec3(0.f, 0.f, fw::Random::GetFloat(-150.f, -100.f));
			}

			if (!m_meteors.empty())
			{
				m_meteors.back()->SetState(true);
				m_meteors.back()->SetPosition(randPos);
				m_meteors.back()->ApplyImpulse(randDirect);
				m_meteors.back()->ApplyTorque(randTorque);
				m_Objects.push_back(m_meteors.back());
				m_meteors.pop_back();
			}
			m_meteorTimer = c_meteorSpawnDelay;
		}
	}

	if (m_spawnedDebris)
	{
		m_debrisTimer -= deltaTime;

		if (m_debrisTimer <= 0.f)
		{
			ResetDebrisPool();
			m_debrisTimer = c_debrisLifeSpan;
		}
	}
}

void Assignment1Scene::ReloadScene()
{
	ResetGame();
	m_showStart = true;
}

fw::GameObject* Assignment1Scene::CheckCollision(fw::CollisionEvent* pCollisionEvent, std::string nameOne, std::string nameTwo)
{
	fw::ContactState contactState = pCollisionEvent->GetContactState();
	fw::GameObject* collisObjOne = pCollisionEvent->GetGameObjectOne();
	fw::GameObject* collisObjTwo = pCollisionEvent->GetGameObjectTwo();

	if ((collisObjOne->GetName() == nameOne && collisObjTwo->GetName() == nameTwo))
	{
		return collisObjOne;
	}
	if (collisObjOne->GetName() == nameTwo && collisObjTwo->GetName() == nameOne)
	{
		return collisObjTwo;
	}

	return nullptr;
}

void Assignment1Scene::SpawnDebris(vec3 pos)
{
	std::vector<fw::GameObject*>::iterator it;
	for (it = m_debris.begin(); it != m_debris.end();)
	{
		vec3 randDirect = vec3(fw::Random::GetFloat(-4.f, 4.f), 4.f, 0.f);
		vec3 randTorque = vec3(0.f, 0.f, fw::Random::GetFloat(10.f, 50.f));

		bool coinFlip = fw::Random::GetInt(0, 1);
		if (coinFlip)
		{
			randTorque = vec3(0.f, 0.f, fw::Random::GetFloat(-50.f, -10.f));
		}

		(*it)->SetState(true);
		(*it)->SetPosition(pos);
		(*it)->ApplyImpulse(randDirect);
		(*it)->ApplyTorque(randTorque);
		m_Objects.push_back((*it));
		it = m_debris.erase(it);
	}

	m_spawnedDebris = true;
}

void Assignment1Scene::SetupPlatform()
{
	fw::GameObject* pPlatform = new fw::GameObject(this, c_centerOfScreen + vec3(0.f, -5.f, 0.f), vec3(0.f, 0.f, 0.f));
	
	fw::MeshComponent* pPlatformMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Platform"), m_pResourceManager->GetMaterial("PlatformCenter"));
	
	//pPlatformMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_02")->uvScale);
	//pPlatformMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_02")->uvOffset);
	
	pPlatform->AddComponent(pPlatformMesh);
	pPlatform->SetScale(vec3(20.f, 2.f, 0.f));
	pPlatform->CreateBody(m_pPhysicsWorld, false, vec3(20.0f, 2.0f, 2.0f), 1.f);
	pPlatform->SetName("Platform");
	m_Objects.push_back(pPlatform);
	
	fw::GameObject* pLeftEdge = new fw::GameObject(this, c_centerOfScreen + vec3(-10.9f, -5.f, 0.f), vec3());
	
	fw::MeshComponent* pLeftEdgeMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));
	
	pLeftEdgeMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_01")->uvScale);
	pLeftEdgeMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_01")->uvOffset);
	pLeftEdge->AddComponent(pLeftEdgeMesh);
	pLeftEdge->SetScale(vec3(2.f, 2.f, 0.f));
	pLeftEdge->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
	pLeftEdge->SetName("Platform");
	m_Objects.push_back(pLeftEdge);
	
	fw::GameObject* pRightEdge = new fw::GameObject(this, c_centerOfScreen + vec3(10.9f, -5.f, 0.f), vec3());
	
	fw::MeshComponent* pRightEdgeMesh = new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"));
	
	pRightEdgeMesh->SetUVScale(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_03")->uvScale);
	pRightEdgeMesh->SetUVOffset(m_pResourceManager->GetSpriteSheet("NiceDaysWalk")->GetSpriteByName("Ground_03")->uvOffset);
	pRightEdge->AddComponent(pRightEdgeMesh);
	pRightEdge->SetScale(vec3(2.f, 2.f, 0.f));
	pRightEdge->CreateBody(m_pPhysicsWorld, false, vec3(2.0f, 2.0f, 2.0f), 1.f);
	pRightEdge->SetName("Platform");
	m_Objects.push_back(pRightEdge);
}

void Assignment1Scene::FillDebrisPool()
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
		pDebris->SetState(false);
		m_debris.push_back(pDebris);
	}
}

void Assignment1Scene::FillMeteorPool()
{
	for (int i = 0; i < 10; i++)
	{
		Meteor* pMeteor = new Meteor(this, m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("NiceDaysWalk"), m_pResourceManager->GetSpriteSheet("NiceDaysWalk"), vec2(c_centerOfScreen.x + 5.f, c_centerOfScreen.y), vec3());
		pMeteor->CreateBody(m_pPhysicsWorld, true, 1.f);
		pMeteor->SetName("Meteor");
		pMeteor->SetState(false);
		m_meteors.push_back(pMeteor);
	}
}

void Assignment1Scene::ResetDebrisPool()
{
	std::vector<fw::GameObject*>::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end();)
	{
		if ((*it)->GetName() == "Debris")
		{
			(*it)->SetState(false);
			m_debris.push_back((*it));
			it = m_Objects.erase(it);
		}
		else
		{
			it++;
		}
	}

	m_spawnedDebris = false;
	m_debrisTimer = c_debrisLifeSpan;
}

void Assignment1Scene::ResetMeteorPool()
{
	std::vector<fw::GameObject*>::iterator it;
	for (it = m_Objects.begin(); it != m_Objects.end();)
	{
		if ((*it)->GetName() == "Meteor")
		{
			(*it)->SetState(false);
			m_meteors.push_back((*it));
			it = m_Objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Assignment1Scene::ResetGame()
{
	ResetDebrisPool();
	ResetMeteorPool();

	m_meteorTimer = c_meteorSpawnDelay;
	m_debrisTimer = c_debrisLifeSpan;

	for (fw::GameObject* pObject : m_Objects)
	{
		if (pObject->GetName() == "Shaun the Sheep")
		{
			pObject->SetState(true);
			pObject->SetPosition(vec2(7.5f, 6.0f));
			pObject->SetRotation(vec3());
			static_cast<Shaun*>(pObject)->SetIsOnGround(false);
		}
	}

	m_showWin = false;
	m_showDeath = false;
}

void Assignment1Scene::StartGameWindow()
{
	ImGui::SetNextWindowSize(ImVec2(140, 70), ImGuiCond_Always);
	if (!ImGui::Begin("Ewe Will Die!", &m_showStart))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Start Game", ImVec2(125, 30)))
	{
		m_isPlaying = true;
		m_showStart = false;
	}
	ImGui::PushItemWidth(-10);
	ImGui::End();
}

void Assignment1Scene::DeathWindow()
{
	ImGui::SetNextWindowSize(ImVec2(140, 70), ImGuiCond_Always);
	if (!ImGui::Begin("You Died", &m_showDeath))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Restart", ImVec2(125, 30)))
	{
		ResetGame();
		m_isPlaying = true;
		m_showDeath = false;
	}
	ImGui::End();
}

void Assignment1Scene::WinWindow()
{
	ImGui::SetNextWindowSize(ImVec2(140, 70), ImGuiCond_Always);
	if (!ImGui::Begin("You Won?", &m_showWin))
	{
		ImGui::End();
		return;
	}

	if (ImGui::Button("Restart", ImVec2(125, 30)))
	{
		ResetGame();
		m_isPlaying = true;
		m_showWin = false;
	}
	ImGui::End();
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
			if (ImGui::MenuItem("Disable to Pause Spawning", "", &m_isPlaying)) {}

			if (ImGui::MenuItem("Reset Game", ""))
			{
				ResetGame();
				m_isPlaying = false;
				m_showStart = true;
			}

			if (ImGui::MenuItem("Enable Walking", "", &m_canWalk)) 
			{
				for (fw::GameObject* pObject : m_Objects)
				{
					if (pObject->GetName() == "Shaun the Sheep")
					{
						static_cast<Shaun*>(pObject)->SetWalking(m_canWalk);
					}
				}
			}

			if (ImGui::MenuItem("Shake Camera", ""))
			{
				m_pCamera->ShakeCamera();
			}

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

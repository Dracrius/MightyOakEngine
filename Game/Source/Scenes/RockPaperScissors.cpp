#include "Framework.h"
#include "DefaultSettings.h"

#include "RockPaperScissors.h"
#include "DataTypes.h"
#include "Game.h"

RockPaperScissors::RockPaperScissors(Game* pGame) : fw::Scene(pGame)
{
    pGame->GetFramework()->GetEventManager()->RegisterForEvents(fw::InputEvent::GetStaticEventType(), this);

    m_pCamera = new fw::Camera(this, c_centerOfScreen + (c_cameraOffset / 4));
	m_pCamera->SetAspectRatio(c_aspectRatio);

    m_pSpriteSheet = m_pResourceManager->GetSpriteSheet("RockPaperScissors");

    fw::GameObject* pComputer = new fw::GameObject(this, c_centerOfScreen + vec3(-1.f, 0.f, 0.f), vec3());
    pComputer->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("White")));
    //pComputer->SetScale(vec3(18.8f, 0.f, 10.f));
    pComputer->SetName("Computer");
    m_Objects.push_back(pComputer);

    fw::GameObject* pPlayer = new fw::GameObject(this, c_centerOfScreen + vec3(1.f, 0.f, 0.f), vec3());
    pPlayer->AddComponent(new fw::MeshComponent(m_pResourceManager->GetMesh("Sprite"), m_pResourceManager->GetMaterial("White")));
    //pPlayer->SetScale(vec3(18.8f, 0.f, 10.f));
    pPlayer->SetName("Player");
    m_Objects.push_back(pPlayer);
}

RockPaperScissors::~RockPaperScissors()
{
}

void RockPaperScissors::StartFrame(float deltaTime)
{
}

void RockPaperScissors::OnEvent(fw::Event* pEvent)
{
    if (pEvent->GetEventType() == fw::InputEvent::GetStaticEventType())
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>(pEvent);

        if (pInputEvent->GetDeviceType() == fw::DeviceType::Keyboard)
        {
            if (pInputEvent->GetInputState() == fw::InputState::Pressed)
            {
                fw::GameObject* pPlayer = m_Objects[1];
                fw::MeshComponent* pMesh = pPlayer->GetComponent<fw::MeshComponent>();

                if (pInputEvent->GetID() == 'R' && !m_playerChoose)
                {
                    pMesh->SetUVScale(m_pSpriteSheet->GetSpriteByName("rock")->uvScale);
                    pMesh->SetUVOffset(m_pSpriteSheet->GetSpriteByName("rock")->uvOffset);
                    pMesh->SetMaterial(m_pResourceManager->GetMaterial("RockPaperScissors"));

                    m_playerChoose = true;
                    m_playerPick = 1;
                    m_showResults = true;
                }
                if (pInputEvent->GetID() == 'P' && !m_playerChoose)
                {
                    pMesh->SetUVScale(m_pSpriteSheet->GetSpriteByName("paper")->uvScale);
                    pMesh->SetUVOffset(m_pSpriteSheet->GetSpriteByName("paper")->uvOffset);
                    pMesh->SetMaterial(m_pResourceManager->GetMaterial("RockPaperScissors"));

                    m_playerChoose = true;
                    m_playerPick = 2;
                    m_showResults = true;
                }
                if (pInputEvent->GetID() == 'S' && !m_playerChoose)
                {
                    pMesh->SetUVScale(m_pSpriteSheet->GetSpriteByName("scissors")->uvScale);
                    pMesh->SetUVOffset(m_pSpriteSheet->GetSpriteByName("scissors")->uvOffset);
                    pMesh->SetMaterial(m_pResourceManager->GetMaterial("RockPaperScissors"));

                    m_playerChoose = true;
                    m_playerPick = 3;
                    m_showResults = true;
                }
                if (pInputEvent->GetID() == 'Q')
                {
                    m_timeElapsed = 0.f;
                    m_reaction = 0.f;
                    m_playerChoose = false;
                    m_compChoose = false;
                    m_playerPick = 0;
                    m_compPick = 0;
                    m_showResults = false;

                    pMesh->SetMaterial(m_pResourceManager->GetMaterial("White"));

                    fw::GameObject* pComputer = m_Objects[0];
                    fw::MeshComponent* pCPUMesh = pComputer->GetComponent<fw::MeshComponent>();
                    pCPUMesh->SetMaterial(m_pResourceManager->GetMaterial("White"));
                }
            }
        }
    }
}

void RockPaperScissors::Update(float deltaTime)
{
    static_cast<Game*>(m_pGame)->SetUsingCubeMap(false);

    Scene::Update(deltaTime);

    if (!m_playerChoose)
    {
        m_reaction += deltaTime;
    }

    if (m_timeElapsed < 1.f)
    {
        m_timeElapsed += deltaTime;
    }
    else if (!m_compChoose)
    {
        m_compChoose = true;

        int rand = fw::Random::GetInt(1,3);

        fw::GameObject* pComputer = m_Objects[0];
        fw::MeshComponent* pMesh = pComputer->GetComponent<fw::MeshComponent>();

        m_compPick = rand;

        switch (rand)
        {
        case 1:
            pMesh->SetUVScale(m_pSpriteSheet->GetSpriteByName("rock")->uvScale);
            pMesh->SetUVOffset(m_pSpriteSheet->GetSpriteByName("rock")->uvOffset);
            break;
        case 2:
            pMesh->SetUVScale(m_pSpriteSheet->GetSpriteByName("paper")->uvScale);
            pMesh->SetUVOffset(m_pSpriteSheet->GetSpriteByName("paper")->uvOffset);
            break;
        case 3:
            pMesh->SetUVScale(m_pSpriteSheet->GetSpriteByName("scissors")->uvScale);
            pMesh->SetUVOffset(m_pSpriteSheet->GetSpriteByName("scissors")->uvOffset);
            break;
        }
        pMesh->SetMaterial(m_pResourceManager->GetMaterial("RockPaperScissors"));
    }

    if (m_showResults)
    {
        Results();
    }

    SettingsMenu();
}

void RockPaperScissors::Results()
{
    if (!ImGui::Begin("Results", &m_showResults))
    {
        ImGui::End();
        return;
    }

    if (m_playerPick == m_compPick)
    {
        ImGui::Text("It was a Tie!");
    }
    if ((m_playerPick == 1 && m_compPick == 2) || (m_playerPick == 2 && m_compPick == 3) || (m_playerPick == 3 && m_compPick == 1))
    {
        ImGui::Text("You Lose!");
    }
    if ((m_playerPick == 1 && m_compPick == 3) || (m_playerPick == 2 && m_compPick == 1) || (m_playerPick == 3 && m_compPick == 2))
    {
        ImGui::Text("You Won!");
    }

    float reaction = m_reaction - m_timeElapsed;

    std::string response = "Reaction Time: " + std::to_string(reaction);

    char chars[256];
    strcpy(chars, response.c_str());

    ImGui::Text(chars);

    ImGui::End();
}

void RockPaperScissors::SettingsMenu()
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
            ImGui::MenuItem("Rock Paper Scissors", NULL, false, false);

			ImGui::EndMenu();
		}
        if (ImGui::BeginMenu("Controls"))
        {
            ImGui::Text("Rock     = R");
            ImGui::Text("Paper    = P");
            ImGui::Text("Scissors = S");
            ImGui::Text("Restart  = Q");

            ImGui::EndMenu();
        }
		ImGui::MenuItem("Rock Paper Scissors Scene", NULL, false, false);

		ImGui::EndMainMenuBar();
	}
}

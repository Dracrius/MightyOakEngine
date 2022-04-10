#include "Framework.h"

#include "Scene.h"
#include "Components/ComponentManager.h"
#include "Events/Event.h"
#include "Objects/Camera.h"
#include "Physics/PhysicsWorld.h"
#include "GameObject.h"


namespace fw {
Scene::Scene(GameCore* pGameCore) : m_pGame(pGameCore)
{
    m_pComponentManager = new ComponentManager();
	m_pResourceManager = pGameCore->GetResourceManager();
}

Scene::~Scene()
{
    for (fw::GameObject* pObject : m_Objects)
    {
        delete pObject;
    }

    delete m_pCamera;

    delete m_pComponentManager;

    delete m_pPhysicsWorld;
}
void Scene::OnEvent(Event* pEvent)
{
    if (pEvent->GetEventType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find(m_Objects.begin(), m_Objects.end(), pObject);
        m_Objects.erase(it);

        delete pObject;
    }
}
void Scene::Update(float deltaTime)
{
	if (m_pPhysicsWorld)
	{
		m_pPhysicsWorld->Update(deltaTime);
	}

    m_pComponentManager->Update(deltaTime);

    m_pCamera->Update(deltaTime);

	if (m_showObjectList)
	{
		if (m_showObjectDetails.size() < m_Objects.size())
		{
			m_showObjectDetails.clear();
			m_showObjectDetails.resize(m_Objects.size());
		}

		Editor_ShowObjectList();
	}

	for (int i = 0; i < m_showObjectDetails.size(); i++)
	{
		if (m_showObjectDetails[i])
		{
			Editor_ShowObjectDetails(i);
		}
	}

	if (m_showObjectPopoutList)
	{
		Editor_ShowObjectPopoutList();
	}
}
void Scene::Draw()
{
    m_pComponentManager->Draw(m_pCamera);

	if (m_debugDraw)
	{
		m_pPhysicsWorld->DebugDraw(m_pCamera, m_pResourceManager->GetMaterial("Default"));
	}
}
void Scene::Editor_ShowObjectList()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			if(ImGui::BeginMenu("Scene Objects"))
			{
				if (!m_Objects.empty())
				{
					for (int i = 0; i < m_Objects.size(); i++)
					{
						char name[30];
						sprintf_s(name, 30, "%s", m_Objects[i]->GetName().c_str());

						bool toggle = m_showObjectDetails[i];
						if (ImGui::MenuItem(name, "", &toggle)) { m_showObjectDetails[i] = true; }
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Scene Objects List", "", &m_showObjectPopoutList)) {}

			ImGui::Separator();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Scene::Editor_ShowObjectPopoutList()
{
	//ImGui::SetNextWindowSize(ImVec2(260, 250), ImGuiCond_Always);

	if (!ImGui::Begin("Scene Objects List", &m_showObjectPopoutList))
	{
		ImGui::End();
		return;
	}

	for (int i = 0; i < m_Objects.size(); i++)
	{
		char name[30];
		sprintf_s(name, 30, "%s", m_Objects[i]->GetName().c_str());

		if (ImGui::CollapsingHeader(name))
		{
			m_Objects[i]->Editor_OutputObjectDetails();
		}
	}
	//m_Objects[index]->Editor_OutputObjectDetails();

	ImGui::End();
}

void Scene::Editor_ShowObjectDetails(int index)
{
	//ImGui::SetNextWindowSize(ImVec2(260, 250), ImGuiCond_Always);

	char name[30];
	sprintf_s(name, 30, "%s", m_Objects[index]->GetName().c_str());
	
	bool toggle = m_showObjectDetails[index];
	if (!ImGui::Begin(name, &toggle))
	{
		ImGui::End();
		return;
	}
	m_showObjectDetails[index] = toggle;
	m_Objects[index]->Editor_OutputObjectDetails();

	ImGui::End();
}

} // namespace fw

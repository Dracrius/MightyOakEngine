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

    for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
    {
        fw::GameObject* pObject = *it;
        fw::PhysicsBodyComponent* pPhysicsBody = pObject->GetComponent<fw::PhysicsBodyComponent>();
        if (pPhysicsBody)
        {
            pPhysicsBody->Update(deltaTime);
        }
    }

    m_pCamera->Update(deltaTime);

	if (m_showObjectList)
	{
		Editor_ShowObjectList();

		if (m_showObjectDetails)
		{
			Editor_ShowObjectDetails();
		}
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
			if(ImGui::BeginMenu("Scene Objects")) //, "", &m_showObjectList
			{
				for (GameObject* pObject : m_Objects)
				{
					char name[30];
					sprintf_s(name, 30, "%s", pObject->GetName().c_str());

					if (ImGui::MenuItem(name, "", &m_showObjectDetails))
					{
						m_pEditor_SelectedObject = pObject;

						if (!m_showObjectDetails)
						{
							m_showObjectDetails = !m_showObjectDetails;
						}
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
void Scene::Editor_ShowObjectDetails()
{
	//ImGui::SetNextWindowSize(ImVec2(260, 250), ImGuiCond_Always);

	char name[30];
	sprintf_s(name, 30, "%s Details", m_pEditor_SelectedObject->GetName().c_str());

	if (!ImGui::Begin(name, &m_showObjectDetails))
	{
		ImGui::End();
		return;
	}
	m_pEditor_SelectedObject->Editor_OutputObjectDetails();

	ImGui::End();
}
} // namespace fw

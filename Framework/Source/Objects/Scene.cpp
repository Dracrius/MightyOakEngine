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
    m_pPhysicsWorld->Update(deltaTime);

    for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
    {
        fw::GameObject* pObject = *it;
        pObject->Update(deltaTime);
    }

    m_pCamera->Update(deltaTime);
}
void Scene::Draw()
{
    m_pComponentManager->Draw(m_pCamera);
}
} // namespace fw

#include "Framework.h"

#include "Scene.h"
#include "GameObject.h"
#include "Physics/PhysicsWorld.h"
#include "Events/Event.h"
#include "Objects/Camera.h"


namespace fw {
Scene::Scene(GameCore* pGameCore) : m_pGame(pGameCore)
{
}

Scene::~Scene()
{
    for (fw::GameObject* pObject : m_Objects)
    {
        delete pObject;
    }

    delete m_pPhysicsWorld;

    delete m_pCamera;
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
    for (auto it = m_Objects.begin(); it != m_Objects.end(); it++)
    {
        fw::GameObject* pObject = *it;
        pObject->Draw(m_pCamera);
    }
}
} // namespace fw

#include "CoreHeaders.h"

#include "ComponentManager.h"
#include "Component.h"

namespace fw {

ComponentManager::ComponentManager()
{
}

void ComponentManager::Update(float deltaTime)
{
}

void ComponentManager::Draw(Camera* pCamera)
{
}

void ComponentManager::AddComponent(Component* pComponent)
{
    std::vector<Component*>& list = m_Components[pComponent->GetType()];

    // Assert that the component *was not* already in the list.
    assert(std::find(list.begin(), list.end(), pComponent) == list.end());

    m_Components[pComponent->GetType()].push_back( pComponent );
}

void ComponentManager::RemoveComponent(Component* pComponent)
{
    std::vector<Component*>& list = m_Components[pComponent->GetType()];

    // Assert that the component *was* in the list.
    assert(std::find(list.begin(), list.end(), pComponent) != list.end());

    list.erase(std::remove(list.begin(), list.end(), pComponent), list.end());
}

} // namespace fw

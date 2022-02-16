#include "Framework.h"

#include "GameObject.h"
#include "Math/Matrix.h"
#include "Material.h"
#include "Components/MeshComponent.h"

namespace fw {

GameObject::GameObject(Scene* pScene, vec3 pos, vec3 rot)
    : m_pScene(pScene)
    , m_Position( pos )
    , m_Rotation( rot )
{
    //if (pMesh != nullptr)
    //{
    //    m_pMeshComponent = new MeshComponent(pMesh, pMaterial);
    //    m_pMeshComponent->SetGameObject(this);
    //    pScene->GetComponentManager()->AddComponent(m_pMeshComponent);
    //}
}

GameObject::~GameObject()
{
    delete m_pPhysicsBody;

    for (auto pComponent : m_pComponents)
    {
        if (pComponent != nullptr)
        {
            m_pScene->GetComponentManager()->RemoveComponent(pComponent);
            delete pComponent;
        }
    }
}

void GameObject::Update(float deltaTime)
{
    if (m_pPhysicsBody)
    {
        m_Position = m_pPhysicsBody->GetPosition();
        m_Rotation = m_pPhysicsBody->GetRotation();
    }
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, m_Scale, density, m_Position, m_Rotation);
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, size, density, m_Position, m_Rotation);
}

void GameObject::AddComponent(Component* pComponent)
{
    pComponent->SetGameObject(this);
    m_pScene->GetComponentManager()->AddComponent(pComponent);

    m_pComponents.push_back(pComponent);
}

void GameObject::RemoveComponent(Component* pComponent)
{
}

Component* GameObject::GetComponent(const char* component)
{
	for (auto pComponent : m_pComponents)
	{
		if (pComponent->GetType() == component)
		{
			return pComponent;
		}
	}
    return nullptr;
}

const matrix& GameObject::GetWorldTransform()
{
    m_WorldTransform.CreateSRT(m_Scale, m_Rotation, m_Position);
    return m_WorldTransform;
}

} // namespace fw

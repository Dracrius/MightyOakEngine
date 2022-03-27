#include "CoreHeaders.h"

#include "PhysicsBodyComponent.h"
#include "Objects/GameObject.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld.h"

namespace fw {

PhysicsBodyComponent::PhysicsBodyComponent() : Component()
{
}

PhysicsBodyComponent::~PhysicsBodyComponent()
{
    delete m_pPhysicsBody;
}

void PhysicsBodyComponent::Update(float deltaTime)
{
    TransformComponent* pTransform = m_pGameObject->GetTransform();

    if (m_pPhysicsBody)
    {
        pTransform->SetPosition(m_pPhysicsBody->GetPosition());
        pTransform->SetRotation(m_pPhysicsBody->GetRotation());
    }
}

void PhysicsBodyComponent::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density)
{
    TransformComponent* pTransform = m_pGameObject->GetTransform();

    CreateBody(pWorld, isDynamic, pTransform->GetScale(), density);
}

void PhysicsBodyComponent::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float radius, float density)
{
    TransformComponent* pTransform = m_pGameObject->GetTransform();
    m_pPhysicsBody = pWorld->CreateBody(m_pGameObject, isDynamic, radius, density, pTransform->GetPosition(), pTransform->GetRotation());

    if (!!m_pGameObject->GetState())
    {
        m_pPhysicsBody->SetState(false);
    }
}

void PhysicsBodyComponent::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    TransformComponent* pTransform = m_pGameObject->GetTransform();
    m_pPhysicsBody = pWorld->CreateBody(m_pGameObject, isDynamic, size, density, pTransform->GetPosition(), pTransform->GetRotation());

    if (!m_pGameObject->GetState())
    {
        m_pPhysicsBody->SetState(false);
    }
}

void PhysicsBodyComponent::ApplyImpulse(const vec3& impulse)
{
    if (m_pPhysicsBody)
    {
        m_pPhysicsBody->ApplyLinearImpulse(impulse, true);
    }
}

void PhysicsBodyComponent::ApplyTorque(const vec3& torque)
{
    if (m_pPhysicsBody)
    {
        m_pPhysicsBody->ApplyTorque(torque, true);
    }
}

} // namespace fw

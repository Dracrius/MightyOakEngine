#include "Framework.h"

#include "GameObject.h"
#include "Math/Matrix.h"
#include "Material.h"
#include "Components/MeshComponent.h"

namespace fw {

GameObject::GameObject(Scene* pScene, Mesh* pMesh, Material* pMaterial, vec3 pos, vec3 rot)
    : m_pScene(pScene)
    , m_Position( pos )
    , m_Rotation( rot )
{
    m_pMeshComponent = new MeshComponent(pMesh, pMaterial);
}

GameObject::~GameObject()
{
    delete m_pPhysicsBody;
    delete m_pMeshComponent;
}

void GameObject::Update(float deltaTime)
{
    if (m_pPhysicsBody)
    {
        m_Position = m_pPhysicsBody->GetPosition();
        m_Rotation = m_pPhysicsBody->GetRotation();
    }
}

void GameObject::Draw(Camera* pCamera)
{
    matrix worldMat;
    worldMat.CreateSRT(m_Scale, m_Rotation, m_Position);

    m_pMeshComponent->Draw(pCamera, worldMat);
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, m_Scale, density, m_Position, m_Rotation);
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, size, density, m_Position, m_Rotation);
}

} // namespace fw

#include "Framework.h"

#include "GameObject.h"
#include "Math/Matrix.h"
#include "Material.h"

namespace fw {

GameObject::GameObject(Scene* pScene, Mesh* pMesh, Material* pMaterial, vec3 pos, vec3 rot)
    : m_pScene(pScene)
    , m_pMesh( pMesh )
    , m_pMaterial(pMaterial)
    , m_Position( pos )
    , m_Rotation( rot )
{
}

GameObject::~GameObject()
{
    delete m_pPhysicsBody;
}

void GameObject::Update(float deltaTime)
{
    if (m_pPhysicsBody)
    {
        vec3 physicsPos = m_pPhysicsBody->GetPosition();
        vec3 physicsRot = m_pPhysicsBody->GetRotation();
        //ImGui::Text("%0.2f, %0.2f", physicsPos.x, physicsPos.y, physicsPos.z);

        m_Position.Set(physicsPos.x, physicsPos.y, physicsPos.z);
        m_Rotation.Set(physicsRot.x, physicsRot.y, physicsRot.z);
    }
}

void GameObject::Draw(Camera* pCamera)
{
    //float rotTime = (float)GetSystemTimeSinceGameStart();
    matrix worldMat;
    worldMat.CreateSRT(m_Scale, m_Rotation, m_Position);

    m_pMesh->Draw( pCamera, m_pMaterial, worldMat, m_UVScale, m_UVOffset, 0.0f );
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, size, density, vec3(m_Position.x - (m_Scale.x / 2), m_Position.y - (m_Scale.y / 2), m_Position.z - (m_Scale.z / 2)));
}

} // namespace fw

#include "Framework.h"

#include "GameObject.h"

namespace fw {

GameObject::GameObject(GameCore* pGame, Mesh* pMesh, ShaderProgram* pShader, Texture* pTexture, vec3 pos)
    : m_pMesh( pMesh )
    , m_pShader( pShader )
    , m_pTexture( pTexture )
    , m_Position( pos )
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
        ImGui::Text("%0.2f, %0.2f", physicsPos.x, physicsPos.y, physicsPos.z);

        m_Position.Set(physicsPos.x, physicsPos.y, physicsPos.z);
    }
}

void GameObject::Draw(Camera* pCamera)
{
    vec3 m_Scale = vec3( 1, 1, 1 );

    m_pMesh->Draw( pCamera, m_pShader, m_pTexture, m_Scale, m_Position, m_UVScale, m_UVOffset, 0.0f );
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, size, density, m_Position);
}

} // namespace fw

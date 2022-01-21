#include "Framework.h"

#include "GameObject.h"

namespace fw {

GameObject::GameObject(GameCore* pGame, Mesh* pMesh, ShaderProgram* pShader, Texture* pTexture, vec2 pos)
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
        b2Vec2 physicsPos = m_pPhysicsBody->GetPosition();
        ImGui::Text("%0.2f, %0.2f", physicsPos.x, physicsPos.y);

        m_Position.Set(physicsPos.x, physicsPos.y);
    }
}

void GameObject::Draw(Camera* pCamera)
{
    vec2 m_Scale = vec2( 1, 1 );

    m_pMesh->Draw( pCamera, m_pShader, m_pTexture, m_Scale, m_Position, m_UVScale, m_UVOffset, 0.0f );
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec2 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(isDynamic, size, density, m_Position);
}

} // namespace fw

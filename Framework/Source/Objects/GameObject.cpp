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
}

void GameObject::Update(float deltaTime)
{
    b2Vec2 physicsPos = m_pPhysicsBody->GetPosition();
    ImGui::Text("%0.2f, %0.2f", physicsPos.x, physicsPos.y);

    m_Position.Set(physicsPos.x, physicsPos.y);
}

void GameObject::Draw(Camera* pCamera)
{
    vec2 m_Scale = vec2( 1, 1 );

    m_pMesh->Draw( pCamera, m_pShader, m_pTexture, m_Scale, m_Position, m_UVScale, m_UVOffset, 0.0f );
}

void GameObject::CreateBody(b2World* pWorld, bool isDynamic, vec2 size, float density)
{
    b2BodyDef bodydef;
    bodydef.position = b2Vec2(m_Position.x, m_Position.y);

    if (isDynamic)
    {
        bodydef.type = b2_dynamicBody;
    }
    else
    {
        bodydef.type = b2_staticBody;
    }

    bodydef.userData.pointer = reinterpret_cast<uintptr_t>(this);

    b2PolygonShape shape;
    shape.SetAsBox(size.x / 2, size.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;

    m_pPhysicsBody = pWorld->CreateBody(&bodydef);
    m_pPhysicsBody->CreateFixture(&fixtureDef);

}

} // namespace fw

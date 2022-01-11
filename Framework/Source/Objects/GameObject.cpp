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
}

void GameObject::Draw(Camera* pCamera)
{
    vec2 m_Scale = vec2( 1, 1 );

    m_pMesh->Draw( pCamera, m_pShader, m_pTexture, m_Scale, m_Position, m_UVScale, m_UVOffset, 0.0f );
}

} // namespace fw

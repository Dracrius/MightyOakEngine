#include "Framework.h"

#include "Cube.h"

Cube::Cube(fw::GameCore* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, fw::Texture* pTexture, vec3 pos)
    : GameObject( pGame, pMesh, pShader, pTexture, pos )
{
    /*m_UVScale = vec2(1.f/6.f, 1.f);
    m_UVOffset = vec2(0, 0);*/
}

Cube::~Cube()
{
}

void Cube::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}


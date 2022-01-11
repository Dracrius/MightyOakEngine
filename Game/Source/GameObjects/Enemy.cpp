#include "Framework.h"

#include "Enemy.h"

Enemy::Enemy(fw::GameCore* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, fw::Texture* pTexture, vec2 pos)
    : GameObject( pGame, pMesh, pShader, pTexture, pos )
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
}

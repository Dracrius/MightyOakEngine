#include "Framework.h"

#include "Enemy.h"

Enemy::Enemy(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, vec3 rot)
    : GameObject(pScene, pos, rot )
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float deltaTime)
{
}

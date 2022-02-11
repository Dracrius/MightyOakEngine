#include "Framework.h"

#include "Cube.h"

Cube::Cube(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec3 pos, vec3 rot)
    : GameObject(pScene, pos, rot )
{
}

Cube::~Cube()
{
}

void Cube::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}


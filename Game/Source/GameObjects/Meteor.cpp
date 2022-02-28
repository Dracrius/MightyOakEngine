#include "Framework.h"

#include "Meteor.h"

Meteor::Meteor(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, vec3 rot)
    : GameObject(pScene, pos, rot )
{
}

Meteor::~Meteor()
{
}

void Meteor::Update(float deltaTime)
{
}

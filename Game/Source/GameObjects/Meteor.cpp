#include "Framework.h"

#include "Meteor.h"

Meteor::Meteor(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, fw::SpriteSheet* pSpriteSheet, vec2 pos, vec3 rot)
    : GameObject(pScene, pos, rot), m_pSpriteSheet(pSpriteSheet)
{
	fw::MeshComponent* pMeshComp = new fw::MeshComponent(pMesh, pMaterial);
	AddComponent(pMeshComp);

	m_Scale = vec3(2.f, 2.f, 0.f);

	int rand = fw::Random::GetInt(10);

	if (rand <= 3)
	{
		pMeshComp->SetUVScale(m_pSpriteSheet->GetSpriteByName("Meteor_01")->uvScale);
		pMeshComp->SetUVOffset(m_pSpriteSheet->GetSpriteByName("Meteor_01")->uvOffset);

		m_radius = 0.56f;
	}
	else
	{
		pMeshComp->SetUVScale(m_pSpriteSheet->GetSpriteByName("Meteor_02")->uvScale);
		pMeshComp->SetUVOffset(m_pSpriteSheet->GetSpriteByName("Meteor_02")->uvOffset);

		m_radius = 1.f;
	}
}

Meteor::~Meteor()
{
}

void Meteor::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}

void Meteor::CreateBody(fw::PhysicsWorld* pWorld, bool isDynamic, float density)
{
	fw::GameObject::CreateBody(pWorld, isDynamic, m_radius, density);
}


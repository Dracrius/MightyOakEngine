#include "Framework.h"

#include "Meteor.h"

Meteor::Meteor(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, fw::SpriteSheet* pSpriteSheet, vec2 pos, vec3 rot)
    : GameObject(pScene, pos, rot), m_pSpriteSheet(pSpriteSheet)
{
	fw::MeshComponent* pMeshComp = new fw::MeshComponent(pMesh, pMaterial);
	AddComponent(pMeshComp);

	int rand = fw::Random::GetInt(10);

	if (rand <= 3)
	{
		pMeshComp->SetUVScale(m_pSpriteSheet->GetSpriteByName("Meteor_01")->uvScale);
		pMeshComp->SetUVOffset(m_pSpriteSheet->GetSpriteByName("Meteor_01")->uvOffset);

		m_radius = 0.56f;
	}
	else
	{
        m_pTransform->SetScale(vec3(2.f, 2.f, 0.f));
		pMeshComp->SetUVScale(m_pSpriteSheet->GetSpriteByName("Meteor_02")->uvScale);
		pMeshComp->SetUVOffset(m_pSpriteSheet->GetSpriteByName("Meteor_02")->uvOffset);

		m_radius = 1.f;
	}
}

Meteor::~Meteor()
{
}


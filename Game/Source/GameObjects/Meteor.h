#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class Meteor : public fw::GameObject
{
protected:
	fw::SpriteSheet* m_pSpriteSheet = nullptr;

	float m_radius = 0.f;

public:
	Meteor(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, fw::SpriteSheet* pSpriteSheet, vec2 pos, vec3 rot);
    virtual ~Meteor();
};

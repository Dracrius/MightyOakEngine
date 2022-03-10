#pragma once
#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class PlayerController;

class SimplePlayer : public fw::GameObject
{
protected:
	PlayerController* m_pPlayerController = nullptr;

public:
	SimplePlayer(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec3 pos, PlayerController* m_pPlayerController);
	~SimplePlayer();

	void Update(float deltaTime);
};


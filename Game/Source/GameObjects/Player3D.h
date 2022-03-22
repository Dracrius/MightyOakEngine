#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class PlayerController;

class Player3D : public fw::GameObject
{
public:
    Player3D(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController);
    virtual ~Player3D();

    virtual void Update(float deltaTime) override;
    //virtual void Draw() override;

    // Setters.

protected:
    PlayerController* m_pPlayerController = nullptr;

    float m_timePassed = 0.f;
};

#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class Enemy : public fw::GameObject
{
public:
    Enemy(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, vec3 rot);
    virtual ~Enemy();

    virtual void Update(float deltaTime) override;
    //virtual void Draw() override;

protected:
};

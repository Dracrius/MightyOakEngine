#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class Cube : public fw::GameObject
{
public:
    Cube(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec3 pos, vec3 rot);
    virtual ~Cube();

    virtual void Update(float deltaTime) override;
    //virtual void Draw(fw::Camera* pCamera) override;

protected:
};

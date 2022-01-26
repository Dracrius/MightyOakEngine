#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class Cube : public fw::GameObject
{
public:
    Cube(fw::GameCore* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, fw::Texture* pTexture, vec3 pos);
    virtual ~Cube();

    virtual void Update(float deltaTime) override;
    //virtual void Draw(fw::Camera* pCamera) override;

protected:
};

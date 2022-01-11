#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

class Enemy : public fw::GameObject
{
public:
    Enemy(fw::GameCore* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, fw::Texture* pTexture, vec2 pos);
    virtual ~Enemy();

    virtual void Update(float deltaTime) override;
    //virtual void Draw() override;

protected:
};

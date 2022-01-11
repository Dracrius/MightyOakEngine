#pragma once

#include "Math/Vector.h"

namespace fw {

class Camera;
class GameCore;
class Mesh;
class ShaderProgram;
class Texture;

class GameObject
{
public:
    GameObject(GameCore* pGame, Mesh* pMesh, ShaderProgram* pShader, Texture* pTexture, vec2 pos);
    virtual ~GameObject();

    virtual void Update(float deltaTime);
    virtual void Draw(Camera* pCamera);

    // Getters.
    vec2 GetPosition() { return m_Position; }

    // Setters.
    void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }

protected:
    Mesh* m_pMesh = nullptr;

    ShaderProgram* m_pShader = nullptr;    
    Texture* m_pTexture = nullptr;
    vec2 m_UVScale = vec2( 1, 1 );
    vec2 m_UVOffset = vec2( 0, 0 );

    vec2 m_Position = vec2( 0, 0 );
};

} // namespace fw

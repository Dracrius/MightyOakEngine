#pragma once
#include "Math/Vector.h"


namespace fw {

class ShaderProgram;
class Texture;

class Material
{
protected:
    ShaderProgram* m_pShader;
    Texture* m_pTexture;
    Color4f m_color;
public:
    Material(ShaderProgram* pShader, Texture* pTexture, Color4f color);
    virtual ~Material();

    // Getters.
    ShaderProgram* GetShader() { return m_pShader; };
    Texture* GetTexture() { return m_pTexture; };
    Color4f GetColor() { return m_color; };

};

} // namespace fw

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
    Texture* m_pCubemap;
public:
    Material(ShaderProgram* pShader, Texture* pTexture, Color4f color, Texture* pCubemap);
    Material(ShaderProgram* pShader, Texture* pTexture, Color4f color);
    Material(ShaderProgram* pShader, Color4f color);
    virtual ~Material();

    // Getters.
    ShaderProgram* GetShader() { return m_pShader; };
    Texture* GetTexture() { return m_pTexture; };
    Color4f GetColor() { return m_color; };
    Texture* GetCubemap() { return m_pCubemap; };

};

} // namespace fw

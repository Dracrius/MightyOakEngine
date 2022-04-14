#include "CoreHeaders.h"
#include "Material.h"
#include "Texture.h"
#include "ShaderProgram.h"

namespace fw {
Material::Material(ShaderProgram* pShader, Texture* pTexture, Color4f color, Texture* pCubemap) : m_pShader(pShader), m_pTexture(pTexture), m_color(color), m_pCubemap(pCubemap)
{
}
Material::Material(ShaderProgram* pShader, Texture* pTexture, Color4f color) : m_pShader(pShader), m_pTexture(pTexture), m_color(color), m_pCubemap(nullptr)
{
}
Material::Material(ShaderProgram* pShader, Color4f color) : m_pShader(pShader), m_pTexture(nullptr), m_color(color), m_pCubemap(nullptr)
{
}
Material::~Material()
{
}
} // namespace fw

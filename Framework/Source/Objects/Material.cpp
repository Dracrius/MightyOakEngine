#include "CoreHeaders.h"
#include "Material.h"
#include "Texture.h"
#include "ShaderProgram.h"

namespace fw {
Material::Material(ShaderProgram* pShader, Texture* pTexture, Color4f color) : m_pShader(pShader), m_pTexture(pTexture), m_color(color)
{
}
Material::~Material()
{
}
} // namespace fw

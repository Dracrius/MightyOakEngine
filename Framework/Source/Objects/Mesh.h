#pragma once

#include "Math/Vector.h"

namespace fw {

class Camera;
class ShaderProgram;
class Texture;

struct VertexFormat
{
    vec2 pos;
    unsigned char color[4];
    vec2 uv;
};

class Mesh
{
public:
    Mesh(GLenum primitiveType, const std::vector<VertexFormat>& verts);
    virtual ~Mesh();

    void SetupUniform(ShaderProgram* pShader, char* name, float value);
    void SetupUniform(ShaderProgram* pShader, char* name, vec2 value);
    void SetupAttribute(ShaderProgram* pShader, char* name, int size, GLenum type, GLboolean normalize, int stride, int64_t startIndex);
    void Draw(Camera* pCamera, ShaderProgram* pShader, Texture* pTexture, vec2 scale, vec2 pos, vec2 uvScale, vec2 uvOffset, float time);

protected:
    GLuint m_VBO = 0;
    GLenum m_PrimitiveType = GL_POINTS;
    int m_NumVerts = 0;
};

} // namespace fw

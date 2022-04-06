#pragma once

#include "Math/Vector.h"

namespace fw {

class Camera;
class ShaderProgram;
class Texture;
class Material;
class matrix;

struct VertexFormat
{
    vec3 pos;
    unsigned char color[4];
    vec2 uv;
    vec3 normal;
};

class Mesh
{
public:
    Mesh();
    Mesh(GLenum primitiveType, const std::vector<VertexFormat>& verts);
    Mesh(GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices);
    virtual ~Mesh();

    void SetupUniform(ShaderProgram* pShader, char* name, float value);
    void SetupUniform(ShaderProgram* pShader, char* name, vec2 value);
    void SetupUniform(ShaderProgram* pShader, char* name, vec3 value);
    void SetupUniform(ShaderProgram* pShader, char* name, vec4 value);
    void SetupUniform(ShaderProgram* pShader, char* name, matrix matrix);
    void SetupAttribute(ShaderProgram* pShader, char* name, int size, GLenum type, GLboolean normalize, int stride, int64_t startIndex);
    void Draw(Camera* pCamera, Material* pMaterial, const matrix& worldMat, vec2 uvScale, vec2 uvOffset, float time);

    void Rebuild(GLenum primitiveType, const std::vector<VertexFormat>& verts);
    void Rebuild(GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices);

    void CreateSprite();
    void CreatePlane(vec2 size, ivec2 vertRes);

    void LoadObj(const char* filename);
    void LoadObj(const char* filename, bool righthanded);

protected:
    GLuint m_VBO = 0;
    GLuint m_IBO = 0;
    GLenum m_PrimitiveType = GL_POINTS;
    int m_NumVerts = 0;
    int m_NumIndices = 0;
};

} // namespace fw

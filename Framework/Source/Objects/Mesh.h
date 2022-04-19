#pragma once

#include "Math/Vector.h"
#include "Components/LightComponent.h"

namespace fw {

class Camera;
class ShaderProgram;
class Texture;
class Material;
class matrix;
class GameObject;

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

    void SetupUniform(ShaderProgram* pShader, char* name, std::vector<float> value);
    void SetupUniform(ShaderProgram* pShader, char* name, std::vector<vec2> value);
    void SetupUniform(ShaderProgram* pShader, char* name, std::vector<vec3> value);
    void SetupUniform(ShaderProgram* pShader, char* name, std::vector<vec4> value);

    void SetupAttribute(ShaderProgram* pShader, char* name, int size, GLenum type, GLboolean normalize, int stride, int64_t startIndex);
    void Draw(GameObject* pParent, Camera* pCamera, Material* pMaterial, const matrix& worldMat, const matrix& normalMat, vec2 uvScale, vec2 uvOffset, float time);

    void FindClosestLights(LightType type, std::vector<Component*>& lights, vec3& objectPos, int index);
    void FillClosestLights(LightType type, std::vector<Component*>& lights);

    void Rebuild(GLenum primitiveType, const std::vector<VertexFormat>& verts);
    void Rebuild(GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices);

    void CreateSprite();
    void CreatePlane(vec2 size, ivec2 vertRes);

    //Made in Class
    void CreatePlane(vec3 topLeftPos, vec2 worldSize, ivec2 vertCount, vec2 uvStart, vec2 uvRange);
    void CreateCylinder(float height, float radius, ivec2 vertCount, vec2 uvStart, vec2 uvRange);
    void CreateSphere(float radius, ivec2 vertCount, vec2 uvStart, vec2 uvRange);

    void LoadObj(const char* filename);
    void LoadObj(const char* filename, bool righthanded);

protected:
    GLuint m_VBO = 0;
    GLuint m_IBO = 0;
    GLenum m_PrimitiveType = GL_POINTS;
    int m_NumVerts = 0;
    int m_NumIndices = 0;

    unsigned int m_numDirecLights = 0;
    unsigned int m_numPointLights = 0;
    unsigned int m_numSpotLights = 0;

    unsigned int m_closestDirecLight = 0;
    std::vector<unsigned int> m_closestPoint{ 0,1,2,3 };
    std::vector<unsigned int> m_closestSpot{ 4,5,6,7 };

    std::vector<vec4>  m_lightColors;
    std::vector<vec3>  m_lightPositions;
    std::vector<vec3>  m_lightRotations;
    std::vector<float> m_lightRadii;
    std::vector<float> m_lightPowerFactors;
    std::vector<float> m_spotCutOffs;
};

} // namespace fw

#include "CoreHeaders.h"

#include "Camera.h"
#include "Mesh.h"
#include "Components/ComponentManager.h"
#include "Objects/Scene.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Material.h"
#include "Utility/Utility.h"
#include "Math/Matrix.h"
#include <stdio.h>

namespace fw {

Mesh::Mesh()
{
}

Mesh::Mesh(GLenum primitiveType, const std::vector<VertexFormat>& verts)
{
    Rebuild(primitiveType, verts);
}

Mesh::Mesh(GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices)
{
    Rebuild(primitiveType, verts, indices);
}

Mesh::~Mesh()
{
    // Release the memory.
    glDeleteBuffers( 1, &m_VBO );
    glDeleteBuffers(1, &m_IBO);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, float value)
{
    GLint location = glGetUniformLocation( pShader->GetProgram(), name );
    glUniform1f( location, value );
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, vec2 value)
{
    GLint location = glGetUniformLocation( pShader->GetProgram(), name );
    glUniform2f( location, value.x, value.y );
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, vec3 value)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform3f(location, value.x, value.y, value.z);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, vec4 value)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, matrix matrix)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniformMatrix4fv(location, 1, false, &matrix.m11);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, std::vector<float> value)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1fv(location, value.size(), &value[0]);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, std::vector<vec2> value)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform2fv(location, value.size(), &value[0].x);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, std::vector<vec3> value)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform3fv(location, value.size(), &value[0].x);
}

void Mesh::SetupUniform(ShaderProgram* pShader, char* name, std::vector<vec4> value)
{
    GLint location = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform4fv(location, value.size(), &value[0].x);
}

void Mesh::SetupAttribute(ShaderProgram* pShader, char* name, int size, GLenum type, GLboolean normalize, int stride, int64_t startIndex)
{
    GLint location = glGetAttribLocation( pShader->GetProgram(), name );
    if( location != -1 )
    {
        glEnableVertexAttribArray( location );
        glVertexAttribPointer( location, size, type, normalize, stride, (void*)startIndex );
    }
}

void Mesh::Draw(GameObject* pParent, Camera* pCamera, Material* pMaterial, const matrix& worldMat, const matrix& normalMat, vec2 uvScale, vec2 uvOffset, float time)
{
    ShaderProgram* pShader = pMaterial->GetShader();
    Texture* pTexture = pMaterial->GetTexture();

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    // Get the attribute variable’s location from the shader.
    // Describe the attributes in the VBO to OpenGL.
    SetupAttribute(pShader, "a_Position", 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), offsetof(VertexFormat, pos));
    SetupAttribute(pShader, "a_Color", 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), offsetof(VertexFormat, color));
    SetupAttribute(pShader, "a_UVCoord", 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), offsetof(VertexFormat, uv));
    SetupAttribute(pShader, "a_Normal", 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), offsetof(VertexFormat, normal));

    // Setup the uniforms.
    glUseProgram(pShader->GetProgram());

    // Matrix uniforms.
    SetupUniform(pShader, "u_WorldMatrix", worldMat);
    SetupUniform(pShader, "u_ViewMatrix", pCamera->GetViewMatrix());
    SetupUniform(pShader, "u_ProjecMatrix", pCamera->GetProjecMatrix());
    SetupUniform(pShader, "u_NormalMatrix", normalMat);

    // UV uniforms.
    SetupUniform( pShader, "u_UVScale", uvScale );
    SetupUniform( pShader, "u_UVOffset", uvOffset );
    
    // Misc uniforms.
    SetupUniform(pShader, "u_Time", (float)GetSystemTimeSinceGameStart());

    SetupUniform(pShader, "u_MaterialColor", vec4(pMaterial->GetColor().r, pMaterial->GetColor().g, pMaterial->GetColor().b, pMaterial->GetColor().a));

    if (pParent)
    {
        vec3 objectPos = pParent->GetTransform()->GetPosition();

        std::vector<Component*>& lights = pCamera->GetScene()->GetComponentManager()->GetComponentsOfType(LightComponent::GetStaticType());

        if (!lights.empty())
        {
            m_numDirecLights = 0;
            m_numPointLights = 0;
            m_numSpotLights = 0;

            for (int i = 0; i < lights.size(); i++)
            {
                LightComponent* light = static_cast<LightComponent*>(lights[i]);
                vec3 lightPos = lights[i]->GetGameObject()->GetPosition();

                switch (light->GetDetails()->type)
                {
                    case LightType::Directional:
                        FindClosestLights(LightType::Directional, lights, objectPos, i);
                        m_numDirecLights++;
                        break;
                    case LightType::PointLight:
                        FindClosestLights(LightType::PointLight, lights, objectPos, i);
                        m_numPointLights++;
                        break;
                    case LightType::SpotLight:
                        FindClosestLights(LightType::SpotLight, lights, objectPos, i);
                        m_numSpotLights++;
                        break;
                }
            }
            
            m_lightColors.clear();
            m_lightPositions.clear();
            m_lightRadii.clear();
            m_lightPowerFactors.clear();

            FillClosestLights(LightType::Directional, lights);
            FillClosestLights(LightType::PointLight, lights);
            FillClosestLights(LightType::SpotLight, lights);

            SetupUniform(pShader, "u_LightColors", m_lightColors);
            SetupUniform(pShader, "u_LightPositions", m_lightPositions);
            SetupUniform(pShader, "u_LightRadii", m_lightRadii);
            SetupUniform(pShader, "u_LightPowerFactors", m_lightPowerFactors);

            SetupUniform(pShader, "u_LightColor", m_lightColors[0]);
            SetupUniform(pShader, "u_LightPos", m_lightPositions[0]);
            SetupUniform(pShader, "u_LightRadius", m_lightRadii[0]);
            SetupUniform(pShader, "u_LightPowerFactor", m_lightPowerFactors[0]);
        }
    }

    GLint hasTexture = glGetUniformLocation(pShader->GetProgram(), "u_HasTexture");

    // Setup textures.
    if (pTexture)
    {
        glUniform1i(hasTexture, true);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
    }
    else
    {
        glUniform1i(hasTexture, false);
    }

    // Draw the primitive.
    if (m_NumIndices > 0)
    {
        glDrawElements(m_PrimitiveType, m_NumIndices, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays( m_PrimitiveType, 0, m_NumVerts );
    }
}

void Mesh::FindClosestLights(LightType type, std::vector<Component*>& lights, vec3& objectPos, int index)
{
    LightComponent* light = static_cast<LightComponent*>(lights[index]);
    vec3 lightPos = light->GetGameObject()->GetPosition();
    unsigned int* closestLights;
    unsigned int closestLightsSize;

    switch (type)
    {
        case LightType::Directional:
            if (index != m_closestDirecLight)
            {
                if (m_numDirecLights == 0 || lightPos.DistanceFrom(objectPos) < lights[m_closestDirecLight]->GetGameObject()->GetPosition().DistanceFrom(objectPos))
                {
                    m_closestDirecLight = index;
                }
            }
            break;
        case LightType::PointLight:
            closestLights = &m_closestPoint[0];
            closestLightsSize = m_closestPoint.size();
        case LightType::SpotLight:
            if (type == LightType::SpotLight)
            {
                closestLights = &m_closestSpot[0];
                closestLightsSize = m_closestSpot.size();
            }
            
            bool inPool = false;
            for (size_t i = 0; i < closestLightsSize; i++)
            {
                if (index == closestLights[i])
                {
                    inPool = true;
                }
            }

            if (!inPool)
            {
                for (size_t i = 0; i < closestLightsSize; i++)
                {
                    if (lightPos.DistanceFrom(objectPos) < lights[closestLights[i]]->GetGameObject()->GetPosition().DistanceFrom(objectPos))
                    {
                        closestLights[i] = index;
                    }
                }
            }
            break;
    }
}

void Mesh::FillClosestLights(LightType type, std::vector<Component*>& lights)
{
    unsigned int* closestLights = nullptr;
    unsigned int closestLightsSize = 0;
    int numDummies = 0;

    switch (type)
    {
        case fw::LightType::Directional:
            closestLights = &m_closestDirecLight;
            closestLightsSize = 1;

            if (m_numDirecLights == 0)
            {
                numDummies = 1;
            }
            break;

        case fw::LightType::PointLight:
            closestLights = &m_closestPoint[0];
            closestLightsSize = m_closestPoint.size();

            if (m_numPointLights < 4)
            {
                numDummies = 4 - m_numPointLights;
            }
            break;

        case fw::LightType::SpotLight:
            closestLights = &m_closestSpot[0];
            closestLightsSize = m_closestSpot.size();

            if (m_numSpotLights < 4)
            {
                numDummies = 4 - m_numSpotLights;
            }
            break;
    }

    LightComponent* light;
    Color4f color;
    vec3 pos;

    Color4f dummyColor = Color4f::Black();
    LightComponent* dummy = new LightComponent(LightType::Directional, dummyColor, 0.0f, 0.0f);

    for (size_t i = 0; i < closestLightsSize; i++)
    {
        if (numDummies > 0)
        {
            light = dummy;
            color = dummyColor;
            numDummies--;
        }
        else
        {
            light = static_cast<LightComponent*>(lights[closestLights[i]]);
            color = light->GetDetails()->diffuse;
            pos = light->GetGameObject()->GetPosition();
        }

        m_lightColors.push_back(vec4(color.r, color.g, color.b, color.a));
        m_lightPositions.push_back(pos);
        m_lightRadii.push_back(light->GetDetails()->radius);
        m_lightPowerFactors.push_back(light->GetDetails()->powerFactor);
    }
}

void Mesh::Rebuild(GLenum primitiveType, const std::vector<VertexFormat>& verts)
{
    glDeleteBuffers(1, &m_VBO);

    m_PrimitiveType = primitiveType;

    m_NumVerts = (int)verts.size();

    // Generate a buffer for our vertex attributes.
    glGenBuffers(1, &m_VBO);

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Copy our attribute data into the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVerts, &verts[0], GL_STATIC_DRAW);
}

void Mesh::Rebuild(GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices)
{
    Rebuild(primitiveType, verts);

    glDeleteBuffers(1, &m_IBO);

    m_NumIndices = (int)indices.size();

    // Generate a buffer for our indices.
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_NumIndices, &indices[0], GL_STATIC_DRAW);
}

void Mesh::CreateSprite()
{
    std::vector<fw::VertexFormat> spriteVerts =
    {
        { vec3(-0.5f,-0.5f,0.f),  255,255,255,255,  vec2(0.0f,0.0f) }, // bottom left
        { vec3(-0.5f,0.5f,0.f),  255,255,255,255,  vec2(0.0f,1.0f) }, // top left
        { vec3(0.5f,-0.5f,0.f),  255,255,255,255,  vec2(1.0f,0.0f) }, // bottom right
        { vec3(0.5f,0.5f,0.f),  255,255,255,255,  vec2(1.0f,1.0f) }, // top right
    };

    std::vector<unsigned int> spriteIndices =
    {
        0, 1, 2, 2, 1, 3,
    };

    Rebuild(GL_TRIANGLES, spriteVerts, spriteIndices);
}

void Mesh::CreatePlane(vec2 size, ivec2 vertRes)
{
    vec2 stepSize = vec2(size.x / (vertRes.x - 1), size.y / (vertRes.y - 1));

    std::vector<fw::VertexFormat> verts;
    std::vector<unsigned int> indices;

    //Centered
    for (int y = 0; y < vertRes.y; y++)
    {
        for (int x = 0; x < vertRes.x; x++)
        {
            vec3 pos = vec3((float(x) - ((vertRes.x / 2) - 0.5f)) * stepSize.x, 0, (float(y) - ((vertRes.y / 2) - 0.5f)) * stepSize.y);

            vec2 uv = vec2(float(x) / float(vertRes.x - 1.f), float(y) / float(vertRes.y - 1.f));

            uv = uv * size / 2;

            verts.push_back({ pos,  255,255,255,255,  uv });
        }
    }

    //Bottom Left
    /*for (int y = 0; y < vertRes.y; y++)
    {
        for (int x = 0; x < vertRes.x; x++)
        {
            vec3 pos = vec3(x * stepSize.x, y * stepSize.y, 0);

            verts.push_back({ pos,  255,255,255,255,  vec2(float(x) / float(vertRes.x - 1.f), float(y) / float(vertRes.y - 1.f)) });
        }
    }*/

    for (int iy = 0; iy < (vertRes.y - 1); iy++)
    {
        for (int ix = 0; ix < (vertRes.x - 1); ix++)
        {
            indices.push_back((iy * vertRes.x) + ix);
            indices.push_back((vertRes.x * (iy + 1)) + ix);
            indices.push_back((vertRes.x * (iy + 1) + 1) + ix);
            indices.push_back((iy * vertRes.x) + ix);
            indices.push_back((vertRes.x * (iy + 1) + 1) + ix);
            indices.push_back((iy * vertRes.x) + 1 + ix);
        }
    }

    Rebuild(GL_TRIANGLES, verts, indices);
}

void Mesh::LoadObj(const char* filename)
{
    LoadObj(filename, false);
}
void Mesh::LoadObj(const char* filename, bool righthanded)
{
    std::vector<vec3> positions;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;

    std::vector<fw::VertexFormat> verts;
    //std::vector<unsigned int> indices;

    long length = 0;
    char* buffer = LoadCompleteFile(filename, &length);
    if (buffer == 0 || length == 0)
    {
        delete[] buffer;
        return;
    }
    char* next_token = 0;
    char* line = strtok_s(buffer, "\n", &next_token);
    while (line)
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            vec3 pos;
            sscanf_s(line, "v %f %f %f", &pos.x, &pos.y, &pos.z);
            if (righthanded)
            {
                pos.z = pos.z * -1.f;
            }
            positions.push_back(pos);
        }
        if (line[0] == 'v' && line[1] == 't')
        {
            vec2 uv;
            sscanf_s(line, "vt %f %f", &uv.x, &uv.y);
            if (righthanded)
            uvs.push_back(uv);
        }
        if (line[0] == 'v' && line[1] == 'n')
        {
            vec3 norm;
            sscanf_s(line, "vn %f %f %f", &norm.x, &norm.y, &norm.z);
            if (righthanded)
            {
                norm.z = norm.z * -1.f;
            }
            normals.push_back(norm);
        }
        if (line[0] == 'f')
        {
            std::vector<int> f(9);
            if (sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7], &f[8]) != 9)
            {
                sscanf_s(line, "f %d//%d %d//%d %d//%d", &f[0], &f[2], &f[3], &f[5], &f[6], &f[8]);
            }

            if (uvs.size() != 0)
            {
                for (int i = 0; i < 3; i++)
                {
                    verts.push_back({ positions[f[0] - 1],  255,255,255,255,  uvs[f[1] - 1], normals[f[2] - 1] });
                    if (righthanded)
                    {
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  uvs[f[7] - 1], normals[f[8] - 1] });
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  uvs[f[4] - 1], normals[f[5] - 1] });
                    }
                    else
                    {
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  uvs[f[4] - 1], normals[f[5] - 1] });
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  uvs[f[7] - 1], normals[f[8] - 1] });
                    }
                    
                }
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    verts.push_back({ positions[f[0] - 1],  255,255,255,255,  vec2(), normals[f[2] - 1] });
                    if (righthanded)
                    {
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  vec2(), normals[f[8] - 1] });
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  vec2(), normals[f[5] - 1] });
                    }
                    else
                    {
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  vec2(), normals[f[5] - 1] });
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  vec2(), normals[f[8] - 1] });
                    }
                }
            }
        }

        //OutputMessage("%s\n", line);
        line = strtok_s(0, "\n", &next_token);
    }

    Rebuild(GL_TRIANGLES, verts);
}
;

} // namespace fw

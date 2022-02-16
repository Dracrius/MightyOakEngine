#include "CoreHeaders.h"

#include "Camera.h"
#include "Mesh.h"
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

void Mesh::SetupAttribute(ShaderProgram* pShader, char* name, int size, GLenum type, GLboolean normalize, int stride, int64_t startIndex)
{
    GLint location = glGetAttribLocation( pShader->GetProgram(), name );
    if( location != -1 )
    {
        glEnableVertexAttribArray( location );
        glVertexAttribPointer( location, size, type, normalize, stride, (void*)startIndex );
    }
}

void Mesh::Draw(Camera* pCamera, Material* pMaterial, const matrix& worldMat, vec2 uvScale, vec2 uvOffset, float time)
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

    // Setup the uniforms.
    glUseProgram(pShader->GetProgram());

    // Matrix uniforms.
    SetupUniform(pShader, "u_WorldMatrix", worldMat);
    SetupUniform(pShader, "u_ViewMatrix", pCamera->GetViewMatrix());
    SetupUniform(pShader, "u_ProjecMatrix", pCamera->GetProjecMatrix());

    // UV uniforms.
    SetupUniform( pShader, "u_UVScale", uvScale );
    SetupUniform( pShader, "u_UVOffset", uvOffset );
    
    // Misc uniforms.
    SetupUniform(pShader, "u_Time", (float)GetSystemTimeSinceGameStart());
    //SetupUniform(pShader, "u_MaterialColor", vec4(15.f / 255, 103.f / 255, 227.f / 255, 1.f));

    SetupUniform(pShader, "u_MaterialColor", vec4(pMaterial->GetColor().r, pMaterial->GetColor().g, pMaterial->GetColor().b, pMaterial->GetColor().a));

    // Setup textures.
    if (pTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Rebuild(GL_TRIANGLES, verts, indices);
}

void Mesh::LoadObj(char* filename)
{
    LoadObj(filename, false);
}
void Mesh::LoadObj(char* filename, bool righthanded)
{
    //CreatePlane(vec2(10.f,10.f), ivec2(2,2));
    std::vector<vec3> positions;
    std::vector <vec2> uvs;
    std::vector <vec3> normals;

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
                    verts.push_back({ positions[f[0] - 1],  255,255,255,255,  uvs[f[1] - 1] }); //normals[f[2] - 1]
                    if (righthanded)
                    {
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  uvs[f[7] - 1] }); //normals[f[8] - 1]
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  uvs[f[4] - 1] }); //normals[f[5] - 1]
                    }
                    else
                    {
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  uvs[f[4] - 1] }); //normals[f[5] - 1]
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  uvs[f[7] - 1] }); //normals[f[8] - 1]
                    }
                    
                }
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    verts.push_back({ positions[f[0] - 1],  255,255,255,255,  vec2() }); //normals[f[2] - 1]
                    if (righthanded)
                    {
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  vec2() }); //normals[f[8] - 1]
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  vec2() }); //normals[f[5] - 1]
                    }
                    else
                    {
                        verts.push_back({ positions[f[3] - 1],  255,255,255,255,  vec2() }); //normals[f[5] - 1]
                        verts.push_back({ positions[f[6] - 1],  255,255,255,255,  vec2() }); //normals[f[8] - 1]
                    }
                }
            }
        }

        OutputMessage("%s\n", line);
        line = strtok_s(0, "\n", &next_token);
    }

    Rebuild(GL_TRIANGLES, verts);
}
;

} // namespace fw

#include "CoreHeaders.h"

#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Utility/Utility.h"
#include "Math/Matrix.h"

namespace fw {

Mesh::Mesh(GLenum primitiveType, const std::vector<VertexFormat>& verts)
    : m_PrimitiveType( primitiveType )
{
    m_NumVerts = (int)verts.size();

    // Generate a buffer for our vertex attributes.
    glGenBuffers( 1, &m_VBO );

    // Set this VBO to be the currently active one.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    // Copy our attribute data into the VBO.
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexFormat)*m_NumVerts, &verts[0], GL_STATIC_DRAW );
}

Mesh::~Mesh()
{
    // Release the memory.
    glDeleteBuffers( 1, &m_VBO );
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

void Mesh::Draw(Camera* pCamera, ShaderProgram* pShader, Texture* pTexture, vec2 scale, vec2 pos, vec2 uvScale, vec2 uvOffset, float time)
{
    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Get the attribute variable’s location from the shader.
    // Describe the attributes in the VBO to OpenGL.
    SetupAttribute(pShader, "a_Position", 2, GL_FLOAT, GL_FALSE, 24, 0);
    SetupAttribute(pShader, "a_Color", 4, GL_UNSIGNED_BYTE, GL_TRUE, 24, 12);
    SetupAttribute(pShader, "a_UVCoord", 2, GL_FLOAT, GL_FALSE, 24, 16);

    // Setup the uniforms.
    glUseProgram(pShader->GetProgram());

    // Matrix uniforms.
    matrix worldMat;
    worldMat.CreateSRT(scale, 0.f, pos);
    SetupUniform(pShader, "u_WorldMatrix", worldMat);

    matrix viewMat;
    viewMat.CreateLookAtView(vec3(pCamera->GetPosition(), -20.f), vec3(0.f, 1.f, 0.f), vec3(pCamera->GetPosition(), 0.f));
    SetupUniform(pShader, "u_ViewMatrix", viewMat);

    matrix projecMat;
    //projecMat.CreateOrtho(-5 ,5, -5, 5, -5, 5);
    projecMat.CreatePerspectiveVFoV(45.f, 1.f, 0.01f, 100.f);
    SetupUniform(pShader, "u_ProjecMatrix", projecMat);

    // UV uniforms.
    SetupUniform( pShader, "u_UVScale", uvScale );
    SetupUniform( pShader, "u_UVOffset", uvOffset );
    
    // Misc uniforms.
    SetupUniform(pShader, "u_Time", (float)GetSystemTimeSinceGameStart());

    // Setup textures.
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, pTexture->GetTextureID() );

    // Draw the primitive.
    glDrawArrays( m_PrimitiveType, 0, m_NumVerts );
}

} // namespace fw

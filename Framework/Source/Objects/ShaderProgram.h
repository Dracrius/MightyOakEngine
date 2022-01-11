#pragma once

namespace fw {

class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(const char* vertFilename, const char* fragFilename);
    virtual ~ShaderProgram();

    // Getters.
    GLuint GetProgram() { return m_Program; }

protected:
    void Cleanup();

    void CompileShader(GLuint& shaderHandle, const char* shaderString);
    bool Init(const char* vertFilename, const char* fragFilename);
    bool Reload();

protected:
    char* m_VertShaderString = nullptr;
    char* m_FragShaderString = nullptr;

    GLuint m_VertShader = 0;
    GLuint m_FragShader = 0;
    GLuint m_Program = 0;
};

} // namespace fw

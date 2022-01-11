#include "CoreHeaders.h"

#include "ShaderProgram.h"
#include "Utility/Utility.h"

namespace fw {

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(const char* vertFilename, const char* fragFilename)
{
    Init( vertFilename, fragFilename );
}

ShaderProgram::~ShaderProgram()
{
    Cleanup();
}

void ShaderProgram::Cleanup()
{
    if( m_VertShaderString )
        delete[] m_VertShaderString;
    if( m_FragShaderString )
        delete[] m_FragShaderString;

    glDetachShader( m_Program, m_VertShader );
    glDetachShader( m_Program, m_FragShader );

    if( m_VertShader )
        glDeleteShader( m_VertShader );
    if( m_FragShader )
        glDeleteShader( m_FragShader );
    if( m_Program )
        glDeleteProgram( m_Program );

    m_VertShaderString = nullptr;
    m_FragShaderString = nullptr;

    m_VertShader = 0;
    m_FragShader = 0;
    m_Program = 0;
}

void ShaderProgram::CompileShader(GLuint& shaderHandle, const char* shaderString)
{
    const char* strings[] = { shaderString };
    glShaderSource( shaderHandle, 1, strings, nullptr );

    glCompileShader( shaderHandle );

    //GLenum errorcode = glGetError();

    int compiled = 0;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &compiled );
    if( compiled == 0 )
    {
        int infoLogLen = 0;
        glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLen );

        char* pBuffer = new char[static_cast<int64_t>(infoLogLen)+1];
        glGetShaderInfoLog( shaderHandle, infoLogLen+1, nullptr, pBuffer );
        OutputMessage( pBuffer );
        assert( false );
        delete[] pBuffer;

        glDeleteShader( shaderHandle );
        shaderHandle = 0;
    }
}

bool ShaderProgram::Init(const char* vertFilename, const char* fragFilename)
{
    m_VertShaderString = LoadCompleteFile( vertFilename, nullptr );
    m_FragShaderString = LoadCompleteFile( fragFilename, nullptr );

    assert( m_VertShaderString != nullptr && m_FragShaderString != nullptr );
    if( m_VertShaderString == nullptr || m_FragShaderString == nullptr )
        return false;

    return Reload();
}

bool ShaderProgram::Reload()
{
    assert( m_VertShaderString != nullptr );
    assert( m_FragShaderString != nullptr );

    m_VertShader = glCreateShader( GL_VERTEX_SHADER );
    m_FragShader = glCreateShader( GL_FRAGMENT_SHADER );

    CompileShader( m_VertShader, m_VertShaderString );
    CompileShader( m_FragShader, m_FragShaderString );

    if( m_VertShader == 0 || m_FragShader == 0 )
    {
        Cleanup();
        return false;
    }

    m_Program = glCreateProgram();
    glAttachShader( m_Program, m_VertShader );
    glAttachShader( m_Program, m_FragShader );

    glLinkProgram( m_Program );

    int linked = 0;
    glGetProgramiv( m_Program, GL_LINK_STATUS, &linked );
    if( linked == 0 )
    {
        int infoLogLen = 0;
        glGetProgramiv( m_Program, GL_INFO_LOG_LENGTH, &infoLogLen );

        char* pBuffer = new char[infoLogLen+1];
        glGetProgramInfoLog( m_Program, infoLogLen+1, nullptr, pBuffer );
        OutputMessage( pBuffer );
        assert( false );
        delete[] pBuffer;

        Cleanup();
        return false;
    }

    return true;
}

} // namespace fw

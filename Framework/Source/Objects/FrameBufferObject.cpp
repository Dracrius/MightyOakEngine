#include "CoreHeaders.h"
#include "FrameBufferObject.h"

namespace fw {

FrameBufferObject::FrameBufferObject(unsigned int width, unsigned int height, std::vector<FBOColorFormat> colorFormats, int depthBits, int minFilter, int magFilter, bool depthReadable)
{
    m_HasValidResources = false;
    m_FullyLoaded = false;
    m_FailedToInit = false;
    m_OnlyFreeOnShutdown = false;

    m_DepthTextureID = 0;

    m_RequestedWidth = 0;
    m_RequestedHeight = 0;

    m_TextureWidth = 0;
    m_TextureHeight = 0;

    m_MinFilter = GL_LINEAR;
    m_MagFilter = GL_LINEAR;

    m_DepthBits = 32;
    m_DepthIsTexture = false;

    m_FrameBufferID = 0;

    Setup( width, height, colorFormats, depthBits, minFilter, magFilter, depthReadable );
    Create();
}

FrameBufferObject::~FrameBufferObject()
{
    Invalidate(true);
}

void FrameBufferObject::Setup(unsigned int width, unsigned int height, std::vector<FBOColorFormat> colorFormats, int depthBits, int minFilter, int magFilter, bool depthReadable)
{
    assert( width <= 8192 );
    assert( height <= 8192 );
    assert( colorFormats.size() <= 4 ); // Search for "Lazy hardcoded limit of 4".

    unsigned int newTextureWidth = 0;
    unsigned int newTextureHeight = 0;

    // Loop from 64 to 8192 and find appropriate size.
    for( unsigned int pow = 6; pow <= 13; pow++ )
    {
        unsigned int powsize = (unsigned int)(1 << pow);

        if (powsize >= width && newTextureWidth == 0)
            newTextureWidth = powsize;
        if (powsize >= height && newTextureHeight == 0)
            newTextureHeight = powsize;
    }

    m_ColorFormats = colorFormats;

    m_RequestedWidth = width;
    m_RequestedHeight = height;

    m_TextureWidth = newTextureWidth;
    m_TextureHeight = newTextureHeight;

    m_MinFilter = minFilter;
    m_MagFilter = magFilter;

    m_DepthBits = 32;
    m_DepthIsTexture = depthReadable;
}

bool FrameBufferObject::Create()
{
    // Don't allow this to be called more than once.
    assert( m_FrameBufferID == 0 );
    assert( m_ColorTextureIDs.size() == 0 && m_DepthTextureID == 0 );

    if( glGenFramebuffers == 0 )
        return false;

    GLint maxSize;
    glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &maxSize );

    if( m_TextureWidth > (unsigned int)maxSize || m_TextureHeight > (unsigned int)maxSize )
    {
        // Requested size is too big.
        return false;
    }

    // Get a framebuffer, render buffer and a texture from opengl.
    glGenFramebuffers( 1, &m_FrameBufferID );
    m_HasValidResources = true;

    // Create the color textures if they're needed.
    for( int i=0; i<m_ColorFormats.size(); i++ )
    {
        m_ColorTextureIDs.push_back( 0 );
        glGenTextures( 1, &m_ColorTextureIDs[i] );
    }

    assert( m_ColorFormats.size() == m_ColorTextureIDs.size() );

    // Create the depth texture if it's needed.
    if( m_DepthBits != 0 )
    {
        assert( m_DepthBits == 16 || m_DepthBits == 24 || m_DepthBits == 32 );

        if( m_DepthIsTexture )
        {
            glGenTextures( 1, &m_DepthTextureID );
        }
        else
        {
            glGenRenderbuffers( 1, &m_DepthTextureID );
        }
    }

    // Create the color textures.
    for( int i=0; i<m_ColorFormats.size(); i++ )
    {
        assert( m_ColorTextureIDs[i] != 0 );

        GLint internalformat = 0;
        GLenum format = 0;
        GLenum type = 0;

        switch( m_ColorFormats[i] )
        {
        case FBOColorFormat_RGBA_UByte:
            internalformat = GL_RGBA;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;

        case FBOColorFormat_RGBA_Float16:
            internalformat = GL_RGBA16F;
            format = GL_RGBA;
            type = GL_FLOAT;
            break;

        case FBOColorFormat_RGB_Float16:
            internalformat = GL_RGB16F;
            format = GL_RGB;
            type = GL_FLOAT;
            break;

        default:
            assert( false );
        }

        assert( format != 0 );

        glBindTexture( GL_TEXTURE_2D, m_ColorTextureIDs[i] );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MinFilter );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MagFilter );
        glTexImage2D( GL_TEXTURE_2D, 0, internalformat, m_TextureWidth, m_TextureHeight, 0, format, type, 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    // Create a depth buffer.
    {
        if( m_DepthTextureID != 0 )
        {
            GLint depthformat = GL_DEPTH_COMPONENT32;
            if( m_DepthBits == 24 )
                depthformat = GL_DEPTH_COMPONENT24;
            else if( m_DepthBits == 16 )
                depthformat = GL_DEPTH_COMPONENT16;

            if( m_DepthIsTexture )
            {
                glBindTexture( GL_TEXTURE_2D, m_DepthTextureID );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
                glTexImage2D( GL_TEXTURE_2D, 0, depthformat, m_TextureWidth, m_TextureHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0 );
                glBindTexture( GL_TEXTURE_2D, 0 );
            }
            else
            {
                glBindRenderbuffer( GL_RENDERBUFFER, m_DepthTextureID );
                glRenderbufferStorage( GL_RENDERBUFFER, depthformat, m_TextureWidth, m_TextureHeight );
            }
        }
    }

    // Attach everything to the FBO.
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferID );

        // Attach color texture.
        for( unsigned int i=0; i<m_ColorTextureIDs.size(); i++ )
        {
            assert( m_ColorTextureIDs[i] != 0 );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorTextureIDs[i], 0 );
        }

        // Attach depth renderbuffer.
        {
            if( m_DepthTextureID )
            {
                if( m_DepthIsTexture )
                {
                    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0 );
                }
                else
                {
                    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTextureID );
                }
            }
        }

        // Any problems?
        GLint status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
        if( status != GL_FRAMEBUFFER_COMPLETE )
        {
            assert( false );
            Invalidate( true );
            return false;
        }

        // Set up the textures for GL to write to. Lazy hardcoded limit of 4.
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers( m_ColorTextureIDs.size(), buffers );
    }

    m_FullyLoaded = true;
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return true;
}

bool FrameBufferObject::Resize(unsigned int width, unsigned int height)
{
    if( width < 64 || height < 64 || width > 4096 || height > 4096 )
        return false;

    if( width < m_TextureWidth && height < m_TextureHeight )
    {
        m_RequestedWidth = width;
        m_RequestedHeight = height;
        return false;
    }
    else
    {
        Invalidate( true );
        Setup( width, height, m_ColorFormats, m_DepthBits, m_MinFilter, m_MagFilter, m_DepthIsTexture );
        Create();
        return true;
    }
}

void FrameBufferObject::Bind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, m_FrameBufferID );
}

void FrameBufferObject::Unbind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void FrameBufferObject::Invalidate(bool cleanGLAllocs)
{
    if( m_HasValidResources == false )
        return;

    if( cleanGLAllocs )
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        for( int i=0; i<m_ColorTextureIDs.size(); i++ )
        {
            if( m_ColorTextureIDs[i] != 0 )
            {
                glDeleteTextures( 1, &m_ColorTextureIDs[i] );
            }
        }
        m_ColorTextureIDs.clear();

        if( m_DepthTextureID != 0 )
        {
            if( m_DepthIsTexture )
                glDeleteTextures( 1, &m_DepthTextureID );
            else
                glDeleteRenderbuffers( 1, &m_DepthTextureID );

            m_DepthTextureID = 0;
        }

        if( m_FrameBufferID != 0 )
        {
            glDeleteFramebuffers( 1, &m_FrameBufferID );
            m_FrameBufferID = 0;
        }
    }
    else
    {
        m_ColorTextureIDs.clear();
        m_DepthTextureID = 0;

        m_FrameBufferID = 0;
    }

    m_FullyLoaded = false;
    m_HasValidResources = false;
}

} // namespace fw

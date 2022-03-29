#pragma once

namespace fw {

class FrameBufferObject
{
public:
    enum FBOColorFormat
    {
        FBOColorFormat_RGBA_UByte,
        FBOColorFormat_RGBA_Float16,
        FBOColorFormat_RGB_Float16,
    };

public:
    FrameBufferObject(unsigned int width, unsigned int height, std::vector<FBOColorFormat> colorFormats, int depthBits = 32, int minFilter = GL_NEAREST, int magFilter = GL_NEAREST, bool depthReadable = true);
    virtual ~FrameBufferObject();

    bool IsFullyLoaded() { return m_FullyLoaded; }
    bool Resize(unsigned int width, unsigned int height);

    void Bind();
    void Unbind();

    GLuint GetColorTextureHandle(int index) { return m_ColorTextureIDs[index]; }
    GLuint GetDepthTextureHandle() { return m_DepthTextureID; }

    unsigned int GetRequestedWidth() { return m_RequestedWidth; }
    unsigned int GetRequestedHeight() { return m_RequestedHeight; }

    unsigned int GetTextureWidth() { return m_TextureWidth; }
    unsigned int GetTextureHeight() { return m_TextureHeight; }

    float GetWidthRatio() { return (float)m_RequestedWidth / m_TextureWidth; }
    float GetHeightRatio() { return (float)m_RequestedHeight / m_TextureHeight; }

protected:
    void Setup(unsigned int width, unsigned int height, std::vector<FBOColorFormat> colorFormats, int depthBits, int minFilter, int magFilter, bool depthReadable);
    void Invalidate(bool cleanGLAllocs);
    bool Create();

protected:
    bool m_HasValidResources;
    bool m_FullyLoaded;

    bool m_FailedToInit;
    bool m_OnlyFreeOnShutdown;

    std::vector<GLuint> m_ColorTextureIDs;
    GLuint m_DepthTextureID;

    unsigned int m_RequestedWidth; // Size requested, mainly used by glViewport call.
    unsigned int m_RequestedHeight;

    unsigned int m_TextureWidth; // Will be power of 2 bigger than requested width/height.
    unsigned int m_TextureHeight;

    int m_MinFilter;
    int m_MagFilter;

    std::vector<FBOColorFormat> m_ColorFormats;
    int m_DepthBits;
    bool m_DepthIsTexture;

    GLuint m_FrameBufferID;
};

} // namespace fw

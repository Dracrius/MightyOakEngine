#pragma once

namespace fw {

class Texture
{
public:
    Texture();
    Texture(const char* filename);
    Texture(std::vector<const char*> filenames);
    virtual ~Texture();

    // Getters.
    GLuint GetTextureID() { return m_TextureID; }

	virtual void SetTexture(const char* filename);
    virtual void SetCubeMapTexture(std::vector<const char*> filenames);

protected:
    GLuint m_TextureID = 0;
};

} // namespace fw

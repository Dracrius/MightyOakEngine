#include "CoreHeaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Libraries/stb/stb_image.h"

#include "Texture.h"

namespace fw {

Texture::Texture()
{
    // Create an array of 4x4 unsigned char's for RGBA
    // Fill it with a checkerboard pattern any 2 colors.
    const int w = 4;
    const int h = 4;
    unsigned char pixels[w*h * 4];
    for( int y=0; y<h; y++ )
    {
        for( int x=0; x<w; x++ )
        {
            int index = (y*w + x)*4;
            int colorIndex = (x+y) % 2;

            if( colorIndex == 0 )
            {
                pixels[index + 0] = 255;
                pixels[index + 1] = 255;
                pixels[index + 2] = 0;
                pixels[index + 3] = 255;
            }
            else
            {
                pixels[index + 0] = 0;
                pixels[index + 1] = 0;
                pixels[index + 2] = 255;
                pixels[index + 3] = 255;
            }
        }
    }

    glGenTextures( 1, &m_TextureID );
    
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, m_TextureID );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

Texture::Texture(const char* filename)
{
    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load( true );
    unsigned char* pixels = stbi_load( filename, &width, &height, &channels, 4 );
    assert( pixels != nullptr );

    glGenTextures( 1, &m_TextureID );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, m_TextureID );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    stbi_image_free( pixels );
}

Texture::~Texture()
{
    glDeleteTextures( 1, &m_TextureID );
}

} // namespace fw

#include "CoreHeaders.h"
#include "SpriteSheet.h"
#include "Utility/Utility.h"

#include "../Libraries/rapidjson/document.h"

namespace fw {

SpriteSheet::SpriteSheet(const char* filename, Texture* pTexture)
	: m_pTexture( pTexture )
{
    const char* jsonString = fw::LoadCompleteFile( filename, nullptr );
    rapidjson::Document document;
    document.Parse( jsonString );
    delete[] jsonString;

    int sheetWidth = document["Width"].GetInt();
    int sheetHeight = document["Height"].GetInt();

    rapidjson::Value& spriteArray = document["Sprites"];
    for( rapidjson::SizeType i=0; i<spriteArray.Size(); i++ )
    {
        rapidjson::Value& sprite = spriteArray[i];

        float x = sprite["X"].GetFloat();
        float y = sprite["Y"].GetFloat();
        float w = sprite["W"].GetFloat();
        float h = sprite["H"].GetFloat();
        
        const char* name = sprite["Name"].GetString();

        m_Sprites[name] = { vec2(w/sheetWidth, h/sheetHeight), vec2(x/sheetWidth, y/sheetHeight) };
    }
}

SpriteSheet::~SpriteSheet()
{
}

SpriteSheet::SpriteInfo SpriteSheet::GetSpriteByName(std::string name)
{
	if( m_Sprites.find( name ) != m_Sprites.end() )
	{
		return m_Sprites.at( name );
	}

	return { vec2(1,1), vec2(0,0) };
}

} // namespace fw

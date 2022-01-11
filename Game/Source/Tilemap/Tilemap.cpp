#include "Framework.h"

#include "Tilemap.h"
#include "Game.h"

Tilemap::Tilemap(Game* pGame, const TileType* pLayout, ivec2 layoutSize, vec2 tileSize)
	: m_LayoutSize( layoutSize )
	, m_TileSize( tileSize )
{
	m_pMesh = pGame->GetMesh( "Sprite" );
	m_pShader = pGame->GetShader( "Basic" );
	m_pSpriteSheet = pGame->GetSpriteSheet( "Sprites" );

	// Setup the tile properties.
	AddTileProperty( false, "", false );        //TT_Empty,
	AddTileProperty( true, "ground_05", true ); //TT_Grass,
	AddTileProperty( true, "ground_06", true ); //TT_Stone,
	AddTileProperty( true, "ground_01", true ); //TT_Dirt,

	// Flip the layout as we copy.
	m_pLayout = new TileType[layoutSize.x * layoutSize.y];
	for( int y=0; y<m_LayoutSize.y; y++ )
	{
		for( int x=0; x<m_LayoutSize.x; x++ )
		{
			int index = y*m_LayoutSize.x + x;
			int flippedIndex = (m_LayoutSize.y - y - 1)*m_LayoutSize.x + x;

			m_pLayout[flippedIndex] = pLayout[index];
		}
	}
}

Tilemap::~Tilemap()
{
}

void Tilemap::Draw(fw::Camera* pCamera)
{
	for( int y=0; y<m_LayoutSize.y; y++ )
	{
		for( int x=0; x<m_LayoutSize.x; x++ )
		{
			int index = y*m_LayoutSize.x + x;

			if( m_TileProperties[ m_pLayout[index] ].hasSprite )
			{
				vec2 pos( x*m_TileSize.x, y*m_TileSize.y );
				vec2 uvScale = m_TileProperties[ m_pLayout[index] ].uvScale;
				vec2 uvOffset = m_TileProperties[ m_pLayout[index] ].uvOffset;

				m_pMesh->Draw( pCamera, m_pShader, m_pSpriteSheet->GetTexture(),
					m_TileSize, pos, uvScale, uvOffset, 0.0f );
			}
		}
	}
}

bool Tilemap::IsWorldPositionWalkable(vec2 worldPos)
{
	if( worldPos.x < 0 || worldPos.y < 0 ||
		worldPos.x >= m_LayoutSize.x*m_TileSize.x || worldPos.y >= m_LayoutSize.y*m_TileSize.y )
	{
		return false;
	}

	ivec2 tilePos = ivec2( (int)(worldPos.x/m_TileSize.x), (int)(worldPos.y/m_TileSize.y) );
	
	int tileIndex = tilePos.y * m_LayoutSize.x + tilePos.x;
	TileType type = m_pLayout[tileIndex];

	return m_TileProperties[type].walkable;
}

void Tilemap::AddTileProperty(bool hasSprite, std::string name, bool walkable)
{
	vec2 uvScale(1,1);
	vec2 uvOffset(0,0);

	if( hasSprite )
	{
		uvScale = m_pSpriteSheet->GetSpriteByName( name ).uvScale;
		uvOffset = m_pSpriteSheet->GetSpriteByName( name ).uvOffset;
	}

	m_TileProperties.push_back( { hasSprite, uvScale, uvOffset, walkable } );
}

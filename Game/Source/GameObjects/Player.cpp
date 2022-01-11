#include "Framework.h"

#include "Player.h"
#include "PlayerController.h"
#include "Tilemap/Tilemap.h"

Player::Player(fw::GameCore* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, fw::Texture* pTexture, vec2 pos, PlayerController* pController)
    : GameObject( pGame, pMesh, pShader, pTexture, pos )
    , m_pPlayerController( pController )
{
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    float speed = 5.0f;

    vec2 newPos = m_Position;

    // Try moving up/down.
    {
        if( m_pPlayerController->IsHeld( PlayerController::Action::Up ) )
        {
            newPos.y += speed * deltaTime;
        }
        if( m_pPlayerController->IsHeld( PlayerController::Action::Down ) )
        {
            newPos.y -= speed * deltaTime;
        }

        if( m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.4f, 0.1f) ) &&
            m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.6f, 0.1f) ) &&
            m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.4f, 0.3f) ) &&
            m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.6f, 0.3f) ) )
        {
            m_Position = newPos;
        }
    }

    // Try moving left/right.
    {
        newPos = m_Position;
        if( m_pPlayerController->IsHeld( PlayerController::Action::Left ) )
        {
            newPos.x -= speed * deltaTime;
        }
        if( m_pPlayerController->IsHeld( PlayerController::Action::Right ) )
        {
            newPos.x += speed * deltaTime;
        }

        if( m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.4f, 0.1f) ) &&
            m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.6f, 0.1f) ) &&
            m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.4f, 0.3f) ) &&
            m_pTilemap->IsWorldPositionWalkable( newPos + vec2(0.6f, 0.3f) ) )
        {
            m_Position = newPos;
        }
    }

    if( m_pPlayerController->WasPressed( PlayerController::Action::Teleport ) )
    {
        m_Position = vec2( rand()/(float)RAND_MAX * 15, rand()/(float)RAND_MAX * 15 );
    }

    if( m_pSpriteSheet )
    {
        fw::SpriteSheet::SpriteInfo info = m_pSpriteSheet->GetSpriteByName( "player_06" );
        m_UVScale = info.uvScale;
        m_UVOffset = info.uvOffset;
    }
}

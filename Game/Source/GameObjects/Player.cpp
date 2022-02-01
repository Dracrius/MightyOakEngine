#include "Framework.h"

#include "Player.h"
#include "PlayerController.h"

Player::Player(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController)
    : GameObject(pScene, pMesh, pMaterial, pos, vec3())
    , m_pPlayerController( pController )
{
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    float speed = 5.0f;

    vec2 newPos = vec2(m_Position.x, m_Position.y);

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
    }

    // Try moving left/right.
    {
        newPos = vec2(m_Position.x, m_Position.y);
        if( m_pPlayerController->IsHeld( PlayerController::Action::Left ) )
        {
            newPos.x -= speed * deltaTime;
        }
        if( m_pPlayerController->IsHeld( PlayerController::Action::Right ) )
        {
            newPos.x += speed * deltaTime;
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

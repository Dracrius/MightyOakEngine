#include "Framework.h"

#include "Player.h"
#include "PlayerController.h"

Player::Player(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController)
    : GameObject(pScene, pos, vec3())
    , m_pPlayerController( pController )
{
    AddComponent(new fw::MeshComponent(pMesh, pMaterial));
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

    m_timePassed += deltaTime;

    if (m_jumpTimer > 0.f)
    {
        m_jumpTimer -= deltaTime;
    }

    float speed = 250.0f;

    // Try moving up/down.
    {
        if( m_pPlayerController->WasPressed( PlayerController::Action::Up ) && m_jumpTimer <= 0.f )
        {
            m_pPhysicsBody->ApplyLinearImpulse(vec3(0.f, speed * deltaTime, 0.f), true);

            m_jumpTimer = 0.5f;
        }
        if( m_pPlayerController->IsHeld( PlayerController::Action::Down ) )
        {
            //Go Through Doors
        }
    }

    // Try moving left/right.
    {
        if( m_pPlayerController->IsHeld( PlayerController::Action::Left ) )
        {
            m_pPhysicsBody->ApplyForce(vec3(-(speed * deltaTime), 0.f, 0.f), true);
        }
        if( m_pPlayerController->IsHeld( PlayerController::Action::Right ) )
        {
            m_pPhysicsBody->ApplyForce(vec3((speed * deltaTime), 0.f, 0.f), true);
        }
    }

    if( m_pPlayerController->WasPressed( PlayerController::Action::Teleport ) )
    {
        m_Position = vec2( rand()/(float)RAND_MAX * 15, rand()/(float)RAND_MAX * 15 );

        m_pPhysicsBody->SetTransform(m_Position, vec3());
    }

    m_velocity = m_pPhysicsBody->GetVelocity();

    if (m_pSpriteSheet)
    {
        CycleAnimFrames(static_cast<int>(m_playerDirections.down.size()));

        fw::MeshComponent* m_pMeshComponent = static_cast<fw::MeshComponent*>(m_pComponent[0]);

        if (m_velocity.x < 0.1f && m_velocity.x > -0.1f)
        {
            m_pMeshComponent->SetUVScale(m_playerDirections.down[m_animFrame]->uvScale);
            m_pMeshComponent->SetUVOffset(m_playerDirections.down[m_animFrame]->uvOffset);
        }
        else if (m_velocity.x > 0.1f)
        {
            m_pMeshComponent->SetUVScale(m_playerDirections.right[m_animFrame]->uvScale);
            m_pMeshComponent->SetUVOffset(m_playerDirections.right[m_animFrame]->uvOffset);
        }
        else if (m_velocity.x < -0.1f)
        {
            m_pMeshComponent->SetUVScale(m_playerDirections.left[m_animFrame]->uvScale);
            m_pMeshComponent->SetUVOffset(m_playerDirections.left[m_animFrame]->uvOffset);
        }
    }
}

void Player::SetAnimations()
{
    std::vector<fw::SpriteSheet::SpriteInfo*> playerUp;
    playerUp.push_back(m_pSpriteSheet->GetSpriteByName("player_08"));
    playerUp.push_back(m_pSpriteSheet->GetSpriteByName("player_09"));
    playerUp.push_back(m_pSpriteSheet->GetSpriteByName("player_10"));
    std::vector<fw::SpriteSheet::SpriteInfo*> playerDown;
    playerDown.push_back(m_pSpriteSheet->GetSpriteByName("player_05"));
    playerDown.push_back(m_pSpriteSheet->GetSpriteByName("player_06"));
    playerDown.push_back(m_pSpriteSheet->GetSpriteByName("player_07"));
    std::vector<fw::SpriteSheet::SpriteInfo*> playerLeft;
    playerLeft.push_back(m_pSpriteSheet->GetSpriteByName("player_20"));
    playerLeft.push_back(m_pSpriteSheet->GetSpriteByName("player_21"));
    playerLeft.push_back(m_pSpriteSheet->GetSpriteByName("player_22"));
    std::vector<fw::SpriteSheet::SpriteInfo*> playerRight;
    playerRight.push_back(m_pSpriteSheet->GetSpriteByName("player_17"));
    playerRight.push_back(m_pSpriteSheet->GetSpriteByName("player_18"));
    playerRight.push_back(m_pSpriteSheet->GetSpriteByName("player_19"));

    m_playerDirections = spriteDirections(playerUp, playerDown, playerLeft, playerRight);

    fw::MeshComponent* m_pMeshComponent = static_cast<fw::MeshComponent*>(m_pComponent[0]);
    //fw::MeshComponent* m_pMeshComponent = static_cast<fw::MeshComponent*>(GetComponent(fw::MeshComponent::GetStaticType());

    m_pMeshComponent->SetUVScale(m_playerDirections.down[m_animFrame]->uvScale);
    m_pMeshComponent->SetUVOffset(m_playerDirections.down[m_animFrame]->uvOffset);
}

void Player::CycleAnimFrames(int numFrames)
{
    float animationLength = 0.12f;

    if (m_animFrame > numFrames - 1)
    {
        m_animFrame = 0;
    }

    if (m_timePassed > animationLength)
    {
        if (m_velocity.x > 0.5f || m_velocity.x < -0.5f)
        {
            if (m_animFrame >= numFrames - 1)
            {
                m_animFrame = 0;
            }
            else
            {
                m_animFrame++;
            }
        }
        else
        {
            m_animFrame = 0;
        }
        m_timePassed = 0.0f;
    }
}

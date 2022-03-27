#include "Framework.h"

#include "Player.h"
#include "PlayerController.h" 
#include "DefaultSettings.h"

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
    m_timePassed += deltaTime;

    if (m_jumpTimer > 0.f)
    {
        m_jumpTimer -= deltaTime;
    }

    fw::PhysicsBody* pPhysicsBody = GetComponent<fw::PhysicsBodyComponent>()->GetPhysicsBody();
    if (pPhysicsBody)
    {
        GetComponent<fw::PhysicsBodyComponent>()->Update(deltaTime);
    }

    //Check Control States
	//Jump
    if( m_pPlayerController->WasPressed( PlayerController::Action::Up ) && m_jumpTimer <= 0.f )
    {
        if (pPhysicsBody)
        {
            pPhysicsBody->ApplyLinearImpulse(vec3(0.f, c_playerSpeed, 0.f), true);
        }
        m_jumpTimer = c_jumpTimer;
    }

	//Activate Switches
    if( m_pPlayerController->IsHeld( PlayerController::Action::Down ) )
    {
        //Go Through Doors
    }

	//Move Left & Right
    if( m_pPlayerController->IsHeld( PlayerController::Action::Left ) )
    {
        if (pPhysicsBody)
        {
            pPhysicsBody->ApplyForce(vec3(-(c_playerSpeed), 0.f, 0.f), true);
        }
    }
    if( m_pPlayerController->IsHeld( PlayerController::Action::Right ) )
    {
        if (pPhysicsBody)
        {
            pPhysicsBody->ApplyForce(vec3(c_playerSpeed, 0.f, 0.f), true);
        }
    }

	//Teleport
    if( m_pPlayerController->WasPressed( PlayerController::Action::Teleport ) )
    {
        m_pTransform->SetPosition(vec2( rand()/(float)RAND_MAX * 15, rand()/(float)RAND_MAX * 15 ));

        if (pPhysicsBody)
        {
            pPhysicsBody->SetTransform(m_pTransform->GetPosition(), vec3());
        }
    }

    if (pPhysicsBody)
    {
        m_velocity = pPhysicsBody->GetVelocity();
    }

	//Set Sprite
    if (m_pSpriteSheet)
    {
        CycleAnimFrames(static_cast<int>(m_playerDirections.down.size()));
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

	fw::MeshComponent* pMesh = GetComponent<fw::MeshComponent>();

    pMesh->SetUVScale(m_playerDirections.down[m_animFrame]->uvScale);
    pMesh->SetUVOffset(m_playerDirections.down[m_animFrame]->uvOffset);
}

void Player::CycleAnimFrames(int numFrames)
{
    if (m_animFrame > numFrames - 1)
    {
        m_animFrame = 0;
    }

    if (m_timePassed > c_animationLength)
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

	fw::MeshComponent* pMesh = GetComponent<fw::MeshComponent>();

	if (m_velocity.x < 0.1f && m_velocity.x > -0.1f)
	{
		pMesh->SetUVScale(m_playerDirections.down[m_animFrame]->uvScale);
		pMesh->SetUVOffset(m_playerDirections.down[m_animFrame]->uvOffset);
	}
	else if (m_velocity.x > 0.1f)
	{
		pMesh->SetUVScale(m_playerDirections.right[m_animFrame]->uvScale);
		pMesh->SetUVOffset(m_playerDirections.right[m_animFrame]->uvOffset);
	}
	else if (m_velocity.x < -0.1f)
	{
		pMesh->SetUVScale(m_playerDirections.left[m_animFrame]->uvScale);
		pMesh->SetUVOffset(m_playerDirections.left[m_animFrame]->uvOffset);
	}
}

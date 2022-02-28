#include "Framework.h"

#include "Shaun.h"
#include "PlayerController.h" 
#include "DefaultSettings.h"

Shaun::Shaun(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController)
    : GameObject(pScene, pos, vec3())
    , m_pPlayerController( pController )
{
    AddComponent(new fw::MeshComponent(pMesh, pMaterial));
}

Shaun::~Shaun()
{
}

void Shaun::Update(float deltaTime)
{
    GameObject::Update(deltaTime);

	m_animationFrameTimer += deltaTime;

	if (m_velocity.x < 0.5f && m_velocity.x > -0.5f)
	{
		if (m_playersLastAction == IdleLeft || m_playersLastAction == WalkLeft || m_playersLastAction == JumpLeft || m_playersLastAction == CrouchLeft)
		{
			m_playersCurrentAction = IdleLeft;
		}
		else
		{
			m_playersCurrentAction = IdleRight;
		}
	}
	else if (m_playersLastAction == JumpLeft)
	{
		if (m_playersCurrentAction != CrouchLeft)
		{
			m_playersCurrentAction = IdleLeft;
		}
	}
	else if (m_playersLastAction == JumpRight)
	{
		if (m_playersCurrentAction != CrouchRight)
		{
			m_playersCurrentAction = IdleRight;
		}
	}

    //Check Control States
	//Jump
    if( m_pPlayerController->WasPressed( PlayerController::Action::Up ) && m_onGround )
    {
        m_pPhysicsBody->ApplyLinearImpulse(vec3(0.f, c_shaunSpeed, 0.f), true);

		if (m_pPhysicsBody->GetVelocity().x > 0.1f || m_playersLastAction == IdleRight)
		{
			m_playersCurrentAction = JumpRight;
		}
		if (m_pPhysicsBody->GetVelocity().x < -0.1f || m_playersLastAction == IdleLeft)
		{
			m_playersCurrentAction = JumpLeft;
		}

		m_onGround = false;
    }

	//Crouch
    if( m_pPlayerController->IsHeld( PlayerController::Action::Down ) )
    {
		if (m_playersLastAction == IdleRight || m_playersLastAction == CrouchRight)
		{
			m_playersCurrentAction = CrouchRight;
		}
		if (m_playersLastAction == IdleLeft || m_playersLastAction == CrouchLeft)
		{
			m_playersCurrentAction = CrouchLeft;
		}
    }

	if (m_canWalk)
	{
		//Walk Left & Right
		if (m_pPlayerController->IsHeld(PlayerController::Action::Left))
		{
			m_pPhysicsBody->ApplyForce(vec3(-(c_shaunSpeed), 0.f, 0.f), true);
			m_playersCurrentAction = WalkLeft;
		}
		if (m_pPlayerController->IsHeld(PlayerController::Action::Right))
		{
			m_pPhysicsBody->ApplyForce(vec3((c_shaunSpeed), 0.f, 0.f), true);
			m_playersCurrentAction = WalkRight;
		}
	}
	else if (m_onGround)
	{
		//Jump Left & Right
		if (m_pPlayerController->WasPressed(PlayerController::Action::Left))
		{
			m_pPhysicsBody->ApplyLinearImpulse(vec3((-c_shaunSpeed / 2.5f), c_shaunSpeed, 0.f), true);
			m_playersCurrentAction = JumpLeft;
			m_onGround = false;
		}
		if (m_pPlayerController->WasPressed(PlayerController::Action::Right))
		{
			m_pPhysicsBody->ApplyLinearImpulse(vec3((c_shaunSpeed / 2.5f), c_shaunSpeed, 0.f), true);
			m_playersCurrentAction = JumpRight;
			m_onGround = false;
		}
	}

	//Teleport
    if( m_pPlayerController->WasPressed( PlayerController::Action::Teleport ) )
    {
        m_Position = vec2( rand()/(float)RAND_MAX * 15, rand()/(float)RAND_MAX * 15 );

        m_pPhysicsBody->SetTransform(m_Position, vec3());

		m_playersCurrentAction = IdleRight;
    }

    m_velocity = m_pPhysicsBody->GetVelocity();

	if (m_playersCurrentAction != m_playersLastAction)
	{
		m_animationTime = 0.f;
	}
	else
	{
		m_animationTime += deltaTime;
	}

	//Set Sprite
    if (m_pSpriteSheet)
    {
        CycleAnimFrames();
    }

	if (m_playersLastAction != JumpLeft && m_playersLastAction != JumpRight)
	{
		m_playersLastAction = m_playersCurrentAction;
	}
	else if(m_playersCurrentAction == CrouchLeft || m_playersCurrentAction == CrouchRight)
	{
		m_playersLastAction = m_playersCurrentAction;
	}
}

void Shaun::SetAnimations()
{
    std::vector<fw::SpriteSheet::SpriteInfo*> shaunCrouch;
	shaunCrouch.push_back(m_pSpriteSheet->GetSpriteByName("Crouch_01"));
	shaunCrouch.push_back(m_pSpriteSheet->GetSpriteByName("Crouch_02"));
    std::vector<fw::SpriteSheet::SpriteInfo*> shaunIRight;
	shaunIRight.push_back(m_pSpriteSheet->GetSpriteByName("Idle_01"));
	shaunIRight.push_back(m_pSpriteSheet->GetSpriteByName("Idle_02"));
	shaunIRight.push_back(m_pSpriteSheet->GetSpriteByName("Idle_03"));
	shaunIRight.push_back(m_pSpriteSheet->GetSpriteByName("Idle_04"));
    std::vector<fw::SpriteSheet::SpriteInfo*> shaunILeft;
	shaunILeft.push_back(m_pSpriteSheet->GetSpriteByName("Idle_05"));
	shaunILeft.push_back(m_pSpriteSheet->GetSpriteByName("Idle_06"));
	shaunILeft.push_back(m_pSpriteSheet->GetSpriteByName("Idle_07"));
	shaunILeft.push_back(m_pSpriteSheet->GetSpriteByName("Idle_08"));
	std::vector<fw::SpriteSheet::SpriteInfo*> shaunWRight;
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_01"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_02"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_03"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_04"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_05"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_06"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_07"));
	shaunWRight.push_back(m_pSpriteSheet->GetSpriteByName("Walk_08"));
	std::vector<fw::SpriteSheet::SpriteInfo*> shaunWLeft;
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_09"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_10"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_11"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_12"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_13"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_14"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_15"));
	shaunWLeft.push_back(m_pSpriteSheet->GetSpriteByName("Walk_16"));

	m_shaunFrames = spriteFrames(shaunCrouch, shaunILeft, shaunIRight, shaunWLeft, shaunWRight);

	m_playersLastAction = IdleRight;

	fw::MeshComponent* pMesh = GetComponent<fw::MeshComponent>();

    pMesh->SetUVScale(m_shaunFrames.idleRight[m_animFrame]->uvScale);
    pMesh->SetUVOffset(m_shaunFrames.idleRight[m_animFrame]->uvOffset);
}

void Shaun::CycleAnimFrames()
{
	int numFrames = 0;
	
	if (m_playersCurrentAction == IdleRight || m_playersCurrentAction == IdleLeft)
	{
		numFrames = 2;
		if (m_animationTime > 9.f)
		{
			m_animationTime = 0.f;
		}

		if (m_animationTime > 5.f)
		{
			numFrames = 4;
		}
		else
		{
			numFrames = 2;
		}
	}
	if (m_playersCurrentAction == CrouchRight || m_playersCurrentAction == CrouchLeft)
	{
		numFrames = 1;
	}
	if (m_playersCurrentAction == WalkRight || m_playersCurrentAction == WalkLeft)
	{
		numFrames = 8;
	}

    if (m_animFrame > numFrames - 1)
    {
		m_animFrame = 0;
		if ((m_playersCurrentAction == IdleRight || m_playersCurrentAction == IdleLeft) && m_animationTime > 5.f)
		{
			m_animFrame = 2;
		}
		else
		{
			m_animFrame = 0;
		}
    }

	fw::MeshComponent* pMesh = GetComponent<fw::MeshComponent>();

	if (m_velocity.x < 0.1f && m_velocity.x > -0.1f)
	{
		if (m_playersCurrentAction != CrouchRight && m_playersCurrentAction != CrouchLeft && m_playersCurrentAction != JumpRight && m_playersCurrentAction != JumpLeft)
		{
			if (m_animFrame > 3)
			{
				m_animFrame = 0;
			}
			if (m_playersLastAction == IdleRight || m_playersLastAction == CrouchRight || m_playersLastAction == WalkRight)
			{
				pMesh->SetUVScale(m_shaunFrames.idleRight[m_animFrame]->uvScale);
				pMesh->SetUVOffset(m_shaunFrames.idleRight[m_animFrame]->uvOffset);
			}
			else if(m_playersLastAction == IdleLeft|| m_playersLastAction == CrouchLeft || m_playersLastAction == WalkLeft)
			{
				pMesh->SetUVScale(m_shaunFrames.idleLeft[m_animFrame]->uvScale);
				pMesh->SetUVOffset(m_shaunFrames.idleLeft[m_animFrame]->uvOffset);
			}
			else if (m_playersLastAction == JumpRight)
			{
				pMesh->SetUVScale(m_shaunFrames.idleRight[0]->uvScale);
				pMesh->SetUVOffset(m_shaunFrames.idleRight[0]->uvOffset);
			}
			else if (m_playersLastAction == JumpLeft )
			{
				pMesh->SetUVScale(m_shaunFrames.idleLeft[0]->uvScale);
				pMesh->SetUVOffset(m_shaunFrames.idleLeft[0]->uvOffset);
			}
		}
		else
		{
			if (m_playersCurrentAction == CrouchRight || m_playersCurrentAction == JumpRight)
			{
				pMesh->SetUVScale(m_shaunFrames.crouch[0]->uvScale);
				pMesh->SetUVOffset(m_shaunFrames.crouch[0]->uvOffset);
			}
			else
			{
				pMesh->SetUVScale(m_shaunFrames.crouch[1]->uvScale);
				pMesh->SetUVOffset(m_shaunFrames.crouch[1]->uvOffset);
			}
		}
	}
	else if (m_velocity.x > 0.1f)
	{
		if (m_playersCurrentAction == JumpRight || (m_playersCurrentAction == CrouchRight && m_playersLastAction == JumpRight))
		{
			pMesh->SetUVScale(m_shaunFrames.crouch[0]->uvScale);
			pMesh->SetUVOffset(m_shaunFrames.crouch[0]->uvOffset);
		}
		else if (m_playersLastAction == JumpRight && m_onGround == false)
		{
			if (m_animFrame > 3)
			{
				m_animFrame = 0;
			}
			pMesh->SetUVScale(m_shaunFrames.idleRight[0]->uvScale);
			pMesh->SetUVOffset(m_shaunFrames.idleRight[0]->uvOffset);
		}
		else if (m_playersCurrentAction == WalkRight || m_playersCurrentAction == IdleRight)
		{
			if (m_animFrame > 7)
			{
				m_animFrame = 0;
			}
			pMesh->SetUVScale(m_shaunFrames.walkRight[m_animFrame]->uvScale);
			pMesh->SetUVOffset(m_shaunFrames.walkRight[m_animFrame]->uvOffset);
		}
	}
	else if (m_velocity.x < -0.1f)
	{
		if (m_playersCurrentAction == JumpLeft || (m_playersCurrentAction == CrouchLeft && m_playersLastAction == JumpLeft))
		{
			pMesh->SetUVScale(m_shaunFrames.crouch[1]->uvScale);
			pMesh->SetUVOffset(m_shaunFrames.crouch[1]->uvOffset);
		}
		else if (m_playersLastAction == JumpLeft && m_onGround == false)
		{
			if (m_animFrame > 3)
			{
				m_animFrame = 0;
			}
			pMesh->SetUVScale(m_shaunFrames.idleLeft[0]->uvScale);
			pMesh->SetUVOffset(m_shaunFrames.idleLeft[0]->uvOffset);
		}
		else if (m_playersCurrentAction == WalkLeft || m_playersCurrentAction == IdleLeft)
		{
			if (m_animFrame > 7)
			{
				m_animFrame = 0;
			}
			pMesh->SetUVScale(m_shaunFrames.walkLeft[m_animFrame]->uvScale);
			pMesh->SetUVOffset(m_shaunFrames.walkLeft[m_animFrame]->uvOffset);
		}
	}

	if (m_playersCurrentAction == IdleLeft || m_playersCurrentAction == IdleRight)
	{
		if (m_animationFrameTimer > c_shaunIdleLength)
		{
			m_animFrame++;

			m_animationFrameTimer = 0.f;
		}
	}
	else
	{
		if (m_animationFrameTimer > c_shaunAnimationLength)
		{
			m_animFrame++;

			m_animationFrameTimer = 0.f;
		}
	}
}

void Shaun::SetIsOnGround(bool onGround)
{
	m_onGround = onGround;

	if (m_onGround)
	{
		if (m_playersLastAction == JumpLeft)
		{
			m_playersCurrentAction = CrouchLeft;
		}
		if (m_playersLastAction == JumpRight)
		{
			m_playersCurrentAction = CrouchRight;
		}
	}
}

#include "Framework.h"
#include "SimplePlayer.h"
#include "PlayerController.h"
#include "DefaultSettings.h"

SimplePlayer::SimplePlayer(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec3 pos, PlayerController* pController)
	: GameObject(pScene, pos, vec3())
	, m_pPlayerController(pController)
{
	AddComponent(new fw::MeshComponent(pMesh, pMaterial));
}

SimplePlayer::~SimplePlayer()
{
}

void SimplePlayer::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	//Check Control States
	//Forward
	if (m_pPlayerController->IsHeld(PlayerController::Action::Up))
	{
		m_Position.z += (c_playerSpeed / 50.f) * deltaTime;
	}

	//Backward
	if (m_pPlayerController->IsHeld(PlayerController::Action::Down))
	{
		m_Position.z -= (c_playerSpeed / 50.f) * deltaTime;
	}

	//Move Left & Right
	if (m_pPlayerController->IsHeld(PlayerController::Action::Left))
	{
		m_Position.x -= (c_playerSpeed / 50.f) * deltaTime;
	}
	if (m_pPlayerController->IsHeld(PlayerController::Action::Right))
	{
		m_Position.x += (c_playerSpeed / 50.f) * deltaTime;
	}

}



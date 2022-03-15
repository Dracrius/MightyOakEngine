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

	vec3 pos = m_pTramsform->GetPosition();

	//Check Control States
	//Forward
	if (m_pPlayerController->IsHeld(PlayerController::Action::Up))
	{
		pos.z += (c_playerSpeed / 50.f) * deltaTime;
	}

	//Backward
	if (m_pPlayerController->IsHeld(PlayerController::Action::Down))
	{
		pos.z -= (c_playerSpeed / 50.f) * deltaTime;
	}

	//Move Left & Right
	if (m_pPlayerController->IsHeld(PlayerController::Action::Left))
	{
		pos.x -= (c_playerSpeed / 50.f) * deltaTime;
	}
	if (m_pPlayerController->IsHeld(PlayerController::Action::Right))
	{
		pos.x += (c_playerSpeed / 50.f) * deltaTime;
	}

	m_pTramsform->SetPosition(pos);

}



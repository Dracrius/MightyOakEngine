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

	float speed = 5.0;
	vec3 pos = m_pTramsform->GetPosition();

	//Movement
	float forwardAxis = 0;
	float strafeAxis = 0;

	if (m_pPlayerController->IsHeld(PlayerController::Action::Up))
	{
		forwardAxis += 1;
	}
	if (m_pPlayerController->IsHeld(PlayerController::Action::Down))
	{
		forwardAxis -= 1;
	}
	if (m_pPlayerController->IsHeld(PlayerController::Action::Left))
	{
		strafeAxis -= 1;
	}
	if (m_pPlayerController->IsHeld(PlayerController::Action::Right))
	{
		strafeAxis += 1;
	}

	//Match Movemnet to Camera Rotation
	float camY = (m_pScene->GetCamera()->GetTransform()->GetRotation().y + 90.f) / 180.f * PI;
	vec3 forwardDir(cos(camY), 0, sin(camY));
	vec3 strafeDir(cos(camY-PI / 2), 0, sin(camY - PI / 2));

	vec3 dir = forwardDir * forwardAxis + strafeDir * strafeAxis;
	dir.Normalize();

	pos += dir  * speed * deltaTime;

	//Move
	m_pTramsform->SetPosition(pos);
	m_pTramsform->SetRotation(vec3(0, m_pScene->GetCamera()->GetTransform()->GetRotation().y, 0));
}



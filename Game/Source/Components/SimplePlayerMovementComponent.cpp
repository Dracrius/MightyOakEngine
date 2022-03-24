#include "CoreHeaders.h"

#include "SimplePlayerMovementComponent.h"
#include "../GameObjects/PlayerController.h"
#include "DefaultSettings.h"


SimplePlayerMovementComponent::SimplePlayerMovementComponent(PlayerController* pController): m_pPlayerController(pController)
{
}

SimplePlayerMovementComponent::~SimplePlayerMovementComponent()
{
}

void SimplePlayerMovementComponent::Update(float deltaTime)
{
    vec3 pos = m_pGameObject->GetTransform()->GetPosition();

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
    float camY = (m_pGameObject->GetScene()->GetCamera()->GetTransform()->GetRotation().y + 90.f) / 180.f * PI;
    vec3 forwardDir(cos(camY), 0, sin(camY));
    vec3 strafeDir(cos(camY - PI / 2), 0, sin(camY - PI / 2));

    vec3 dir = forwardDir * forwardAxis + strafeDir * strafeAxis;
    dir.Normalize();

    pos += dir * m_speed * deltaTime;

    //Move
    m_pGameObject->GetTransform()->SetPosition(pos);
    m_pGameObject->GetTransform()->SetRotation(vec3(0, m_pGameObject->GetScene()->GetCamera()->GetTransform()->GetRotation().y, 0));
}

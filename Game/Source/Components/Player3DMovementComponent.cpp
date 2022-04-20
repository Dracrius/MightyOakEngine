#include "CoreHeaders.h"

#include "Player3DMovementComponent.h"
#include "../GameObjects/PlayerController.h"
#include "DefaultSettings.h"


Player3DMovementComponent::Player3DMovementComponent(PlayerController* pController): m_pPlayerController(pController)
{
}

Player3DMovementComponent::~Player3DMovementComponent()
{
}

void Player3DMovementComponent::Update(float deltaTime)
{
    if (m_firstUpdate)
    {
        m_origRotation = m_pGameObject->GetTransform()->GetRotation();
        m_firstUpdate = false;
    }

    vec3 pos = m_pGameObject->GetTransform()->GetPosition();

    fw::PhysicsBodyComponent* pPhysicsBody = m_pGameObject->GetComponent<fw::PhysicsBodyComponent>();

    m_timePassed += deltaTime;

    float speed = 5.0;

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

    //Move
    if (pPhysicsBody)
    {
        pPhysicsBody->Update(deltaTime);
        pPhysicsBody->GetPhysicsBody()->ApplyForce(dir * speed, true);
        //pPhysicsBody->GetPhysicsBody()->ApplyTorque(vec3(0, strafeAxis, 0), true); //Camera would need to adjust
    }
}

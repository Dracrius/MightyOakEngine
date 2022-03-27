#include "Framework.h"

#include "Player3D.h"
#include "PlayerController.h" 
#include "DefaultSettings.h"

Player3D::Player3D(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController)
    : GameObject(pScene, pos, vec3())
    , m_pPlayerController( pController )
{
    AddComponent(new fw::MeshComponent(pMesh, pMaterial));
}

Player3D::~Player3D()
{
}

void Player3D::Update(float deltaTime)
{
    fw::PhysicsBodyComponent* pPhysicsBody = GetComponent<fw::PhysicsBodyComponent>();
    if (pPhysicsBody)
    {
        pPhysicsBody->Update(deltaTime);
    }

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
    float camY = (m_pScene->GetCamera()->GetTransform()->GetRotation().y + 90.f) / 180.f * PI;
    vec3 forwardDir(cos(camY), 0, sin(camY));
    vec3 strafeDir(cos(camY - PI / 2), 0, sin(camY - PI / 2));

    vec3 dir = forwardDir * forwardAxis; // +strafeDir * strafeAxis;
    dir.Normalize();

    //Move
    if (pPhysicsBody)
    {
        pPhysicsBody->Update(deltaTime);
        pPhysicsBody->GetPhysicsBody()->ApplyForce(dir * speed, true);
        //pPhysicsBody->GetPhysicsBody()->ApplyTorque(vec3(0, strafeAxis, 0), true); //Camera would need to adjust
    }

}

#pragma once

#include "Framework.h"

class PlayerController;

class Player3DMovementComponent : public fw::Component
{
protected:
    PlayerController* m_pPlayerController = nullptr;
    float m_speed = 5.f;

    fw::vec3 m_origRotation;
    bool m_firstUpdate = true;

    float m_timePassed = 0.f;

public:
    Player3DMovementComponent(PlayerController* pController);
    virtual ~Player3DMovementComponent();

    void Update(float deltaTime);

    static const char* GetStaticType() { return "Player3DMovementComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
};


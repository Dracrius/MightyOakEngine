#pragma once

#include "Framework.h"


class PlayerController;

class SimplePlayerMovementComponent : public fw::Component
{
protected:
    PlayerController* m_pPlayerController = nullptr;
    float m_speed = 5.f;

public:
    SimplePlayerMovementComponent(PlayerController* pController);
    virtual ~SimplePlayerMovementComponent();

    void Update(float deltaTime);

    static const char* GetStaticType() { return "SimplePlayerMovementComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
};


#pragma once

#include "Framework.h"

class PlayerController
{
public:
    enum Action
    {
        Up       = 1 << 0,
        Down     = 1 << 1,
        Left     = 1 << 2,
        Right    = 1 << 3,
        Teleport = 1 << 4,
    };

public:
    PlayerController();

    void StartFrame();
    void OnEvent(fw::Event* pEvent);

    bool IsHeld(Action action);
    bool WasPressed(Action action);
    bool WasReleased(Action action);

protected:
    unsigned int m_OldFlags = 0;
    unsigned int m_Flags = 0;
};

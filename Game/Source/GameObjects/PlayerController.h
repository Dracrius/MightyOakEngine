#pragma once

#include "Framework.h"

class PlayerController : public fw::EventListener
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
    PlayerController(fw::EventManager* pEventManager);
	~PlayerController();

    void StartFrame();
    virtual void OnEvent(fw::Event* pEvent) override;

    bool IsHeld(Action action);
    bool WasPressed(Action action);
    bool WasReleased(Action action);

protected:
	fw::EventManager* m_pEventManager;

    unsigned int m_OldFlags = 0;
    unsigned int m_Flags = 0;
};

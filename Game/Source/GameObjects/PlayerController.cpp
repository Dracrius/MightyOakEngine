#include "Framework.h"

#include "PlayerController.h"

PlayerController::PlayerController(fw::EventManager* pEventManager)
{
	if (pEventManager)
	{
		m_pEventManager = pEventManager;
		m_pEventManager->RegisterForEvents(fw::InputEvent::GetStaticEventType(), this);
	}
}

PlayerController::~PlayerController()
{
	if (m_pEventManager)
	{
		m_pEventManager->UnregisterForEvents(fw::InputEvent::GetStaticEventType(), this);
	}
}

void PlayerController::StartFrame()
{
    m_OldFlags = m_Flags;
}

void PlayerController::OnEvent(fw::Event* pEvent)
{
    if( pEvent->GetEventType() == fw::InputEvent::GetStaticEventType() )
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>( pEvent );

        if( pInputEvent->GetDeviceType() == fw::DeviceType::Keyboard )
        {
            if( pInputEvent->GetInputState() == fw::InputState::Pressed )
            {
                if( pInputEvent->GetID() == 'W' || pInputEvent->GetID() == VK_SPACE )      { m_Flags |= Action::Up; }
                if( pInputEvent->GetID() == 'S' )    { m_Flags |= Action::Down; }
                if( pInputEvent->GetID() == 'A' )    { m_Flags |= Action::Left; }
                if( pInputEvent->GetID() == 'D' )   { m_Flags |= Action::Right; }
                if( pInputEvent->GetID() == 'Z' )                                       { m_Flags |= Action::Teleport; }
            }

            if( pInputEvent->GetInputState() == fw::InputState::Released )
            {
                if( pInputEvent->GetID() == 'W' || pInputEvent->GetID() == VK_SPACE )      { m_Flags &= ~Action::Up; }
                if( pInputEvent->GetID() == 'S' )    { m_Flags &= ~Action::Down; }
                if( pInputEvent->GetID() == 'A' )    { m_Flags &= ~Action::Left; }
                if( pInputEvent->GetID() == 'D' )   { m_Flags &= ~Action::Right; }
                if( pInputEvent->GetID() == 'Z' )                                       { m_Flags &= ~Action::Teleport; }
            }
        }
    }
}

bool PlayerController::IsHeld(Action action)
{
    return m_Flags & action;
}

bool PlayerController::WasPressed(Action action)
{
    bool wasHeldLastFrame = m_OldFlags & action;
    bool isHeldThisFrame = m_Flags & action;

    return wasHeldLastFrame == false && isHeldThisFrame == true;
}

bool PlayerController::WasReleased(Action action)
{
    bool wasHeldLastFrame = m_OldFlags & action;
    bool isHeldThisFrame = m_Flags & action;

    return wasHeldLastFrame == true && isHeldThisFrame == false;
}

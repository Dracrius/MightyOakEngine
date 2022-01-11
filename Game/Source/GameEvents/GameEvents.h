#pragma once

//===============================================
// RemoveFromGameEvent class
//===============================================
class RemoveFromGameEvent : public fw::Event
{
public:
    RemoveFromGameEvent(fw::GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    // Event Type Getters.
    static const char* GetStaticEventType() { return "RemoveFromGameEvent"; }
    virtual const char* GetEventType() override { return GetStaticEventType(); }

    // Getters.
    fw::GameObject* GetGameObject() { return m_pObject; }

protected:
    fw::GameObject* m_pObject = nullptr;
};

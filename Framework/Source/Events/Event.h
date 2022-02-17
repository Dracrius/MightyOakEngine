#pragma once

#include "Math/Vector.h"

namespace fw {

class GameObject;

enum class DeviceType { Keyboard, Mouse };
enum class InputState { Pressed, Released, Input };

//===============================================
// Event class
//===============================================
class Event
{
public:
    Event() {}
    virtual ~Event() {}

    // Event Type Getters.
    virtual const char* GetEventType() = 0;
};

//===============================================
// InputEvent class
//===============================================
class InputEvent : public Event
{
public:
    InputEvent(DeviceType deviceType, InputState inputState, int keyCode)
        : m_DeviceType( deviceType )
        , m_InputState( inputState )
        , m_ID( keyCode )
    {
    }

    // Event Type Getters.
    static const char* GetStaticEventType() { return "InputEvent"; }
    virtual const char* GetEventType() override { return GetStaticEventType(); }

    // Getters.
    DeviceType GetDeviceType() { return m_DeviceType; }
    InputState GetInputState() { return m_InputState; }
    int GetID() { return m_ID; }
    vec2 GetPosition() { return m_Pos; }

protected:
    DeviceType m_DeviceType;
    InputState m_InputState;
    int m_ID;
    vec2 m_Pos;
};

//===============================================
// RemoveFromGameEvent class
//===============================================
class RemoveFromGameEvent : public Event
{
public:
    RemoveFromGameEvent(GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    // Event Type Getters.
    static const char* GetStaticEventType() { return "RemoveFromGameEvent"; }
    virtual const char* GetEventType() override { return GetStaticEventType(); }

    // Getters.
    GameObject* GetGameObject() { return m_pObject; }

protected:
    GameObject* m_pObject = nullptr;
};

//===============================================
// CollisionEvent class
//===============================================
enum class ContactState { Begin, End };

class CollisionEvent : public Event
{
public:
	CollisionEvent(GameObject* one, GameObject* two, ContactState contactState)
		: m_one(one)
		, m_two(two)
		, m_contactState(contactState)
	{
	}

	// Event Type Getters.
	static const char* GetStaticEventType() { return "CollisionEvent"; }
	virtual const char* GetEventType() override { return GetStaticEventType(); }

	// Getters.
	GameObject* GetGameObjectOne() { return m_one; }
	GameObject* GetGameObjectTwo() { return m_two; }
	ContactState GetContactState() { return m_contactState; }

protected:
	GameObject* m_one;
	GameObject* m_two;
	ContactState m_contactState;
};

} // namespace fw

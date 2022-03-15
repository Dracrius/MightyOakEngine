#pragma once

namespace fw {

class Event;
class GameCore;

class EventListener
{
public:
	virtual void OnEvent(Event* pEvent) = 0;
};

class EventManager
{
public:
    EventManager();
    ~EventManager();

    void AddEvent(Event* pEvent);
    void ProcessEvents();

	void RegisterForEvents(const char* eventType, EventListener* pListener);
	void UnregisterForEvents(const char* eventType, EventListener* pListener);

protected:
    std::queue<Event*> m_eventQueue;

	std::map<const char*, std::vector<fw::EventListener*>> m_eventListeners;
};

} // namespace fw

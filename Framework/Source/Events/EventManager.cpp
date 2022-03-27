#include "CoreHeaders.h"

#include "EventManager.h"
#include "Event.h"
#include "GameCore.h"

namespace fw {

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
    while( m_eventQueue.empty() == false )
    {
        Event* pEvent = m_eventQueue.front();
        m_eventQueue.pop();

        delete pEvent;
    }
}

void EventManager::AddEvent(Event* pEvent)
{
    m_eventQueue.push( pEvent );
}

void EventManager::ProcessEvents()
{
    while( m_eventQueue.empty() == false )
    {
        Event* pEvent = m_eventQueue.front();
        m_eventQueue.pop();

		if(m_eventListeners.find(pEvent->GetEventType()) != m_eventListeners.end())
		{
			for (EventListener* pListener : m_eventListeners[pEvent->GetEventType()])
			{
				pListener->OnEvent(pEvent);
			}
		}

        delete pEvent;
    }
}

void EventManager::RegisterForEvents(const char* eventType, EventListener* pListener)
{
	m_eventListeners[eventType].push_back(pListener);
}

void EventManager::UnregisterForEvents(const char* eventType, EventListener* pListener)
{
	if (m_eventListeners.find(eventType) != m_eventListeners.end())
	{
		std::vector<fw::EventListener*>& list = m_eventListeners[eventType];
		list.erase(std::remove(list.begin(), list.end(), pListener), list.end());
	}
}

} // namespace fw

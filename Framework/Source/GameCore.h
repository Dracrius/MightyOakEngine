#pragma once

#include "Events/EventManager.h"

namespace fw {

class Event;
class ResourceManager;

class GameCore : public EventListener
{
protected:
	ResourceManager* m_pResourceManager = nullptr;
public:
	GameCore();
	virtual ~GameCore();

    virtual void StartFrame(float deltaTime) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

	ResourceManager* GetResourceManager() { return m_pResourceManager; }
};

} // namespace fw

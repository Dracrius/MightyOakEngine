#pragma once

namespace fw {

class Event;
class ResourceManager;

class GameCore
{
protected:
	ResourceManager* m_pResourceManager = nullptr;
public:
	GameCore();
	virtual ~GameCore();

    virtual void StartFrame(float deltaTime) = 0;
    virtual void OnEvent(Event* pEvent) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

	ResourceManager* GetResourceManager() { return m_pResourceManager; }
};

} // namespace fw

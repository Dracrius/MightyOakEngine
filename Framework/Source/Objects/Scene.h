#pragma once
#include "Events/EventManager.h"

namespace fw {

class Camera;
class Event;
class GameCore;
class GameObject;
class PhysicsWorld;
class ComponentManager;
class ResourceManager;

class Scene : public EventListener
{
protected:
    GameCore* m_pGame = nullptr;
	ResourceManager* m_pResourceManager = nullptr;

    Camera* m_pCamera = nullptr;
    PhysicsWorld* m_pPhysicsWorld = nullptr;
    std::vector<GameObject*> m_Objects;

    ComponentManager* m_pComponentManager = nullptr;

	bool m_debugDraw = false;

	bool m_showObjectList = true;
    bool m_showObjectPopoutList = false;
	std::vector<bool> m_showObjectDetails;
public:
    Scene(GameCore* pGameCore);
    virtual ~Scene();

    virtual void StartFrame(float deltaTime) = 0;

    virtual void OnEvent(Event* pEvent);

    virtual void Update(float deltaTime);
    virtual void Draw();

	virtual Camera* GetCamera() { return m_pCamera; }

	virtual void SetDebugDraw(bool state) { m_debugDraw = state; }
	virtual void ToggleDebugDraw() { m_debugDraw = !m_debugDraw; }

	virtual void Editor_SetShowGObjList(bool state) { m_showObjectList = state; }
	virtual void Editor_ToggleShowGObjList() { m_showObjectList = !m_showObjectList; }

	void Editor_ShowObjectList();
    void Editor_ShowObjectPopoutList();
	void Editor_ShowObjectDetails(int index);

    ComponentManager* GetComponentManager() { return m_pComponentManager; }
};

} // namespace fw

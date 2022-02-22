#pragma once

namespace fw {

class Camera;
class Event;
class GameCore;
class GameObject;
class PhysicsWorld;
class ComponentManager;
class ResourceManager;

class Scene
{
protected:
    GameCore* m_pGame = nullptr;
	ResourceManager* m_pResourceManager = nullptr;

    Camera* m_pCamera = nullptr;
    PhysicsWorld* m_pPhysicsWorld = nullptr;
    std::vector<GameObject*> m_Objects;

    ComponentManager* m_pComponentManager = nullptr;
public:
    Scene(GameCore* pGameCore);
    virtual ~Scene();

    virtual void StartFrame(float deltaTime) = 0;

    virtual void OnEvent(Event* pEvent);

    virtual void Update(float deltaTime);
    virtual void Draw();

    ComponentManager* GetComponentManager() { return m_pComponentManager; }
};

} // namespace fw

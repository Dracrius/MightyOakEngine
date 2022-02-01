#pragma once

namespace fw {

class Camera;
class Event;
class GameCore;
class GameObject;
class PhysicsWorld;

class Scene
{
protected:
    GameCore* m_pGame = nullptr;

    Camera* m_pCamera = nullptr;
    PhysicsWorld* m_pPhysicsWorld;
    std::vector<GameObject*> m_Objects;

    //PlayerController* m_pPlayerController = nullptr;
public:
    Scene(GameCore* pGameCore);
    virtual ~Scene();

    virtual void StartFrame(float deltaTime) = 0;

    virtual void OnEvent(Event* pEvent);

    virtual void Update(float deltaTime);
    virtual void Draw();
};

} // namespace fw

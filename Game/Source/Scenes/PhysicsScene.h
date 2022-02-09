#pragma once

class Game;
class PlayerController;

class PhysicsScene : public fw::Scene
{
protected:
    PlayerController* m_pPlayerController = nullptr;
public:
    PhysicsScene(Game* pGame);
    virtual ~PhysicsScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;
};


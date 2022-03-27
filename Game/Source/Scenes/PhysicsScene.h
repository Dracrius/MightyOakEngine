#pragma once

class Game;
class PlayerController;
class Player;

class PhysicsScene : public fw::Scene
{
protected:
    PlayerController* m_pPlayerController = nullptr;

    Player* m_pPlayer = nullptr;

    bool m_showOrthoMouseCoords = false;
    bool m_showPerspecMouseCoords = false;

public:
    PhysicsScene(Game* pGame);
    virtual ~PhysicsScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;


protected:
    void ShowOrthoMouseCoordinates();
    void ShowPerspecMouseCoordinates();

	void ControlsMenu();
};


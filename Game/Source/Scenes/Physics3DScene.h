#pragma once

class Game;
class PlayerController;
class Player3D;

class Physics3DScene : public fw::Scene
{
protected:
    PlayerController* m_pPlayerController = nullptr;

    fw::GameObject* m_pPlayer = nullptr;

    bool m_showPerspecMouseCoords = false;

public:
    Physics3DScene(Game* pGame);
    virtual ~Physics3DScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;

protected:
    void ShowPerspecMouseCoordinates();

	void ControlsMenu();
};


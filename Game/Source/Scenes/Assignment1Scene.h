#pragma once

class Game;
class PlayerController;

class Assignment1Scene : public fw::Scene
{
protected:
    PlayerController* m_pPlayerController = nullptr;

	std::vector<fw::GameObject*> m_meteors;
	std::vector<fw::GameObject*> m_debris;

public:
	Assignment1Scene(Game* pGame);
    virtual ~Assignment1Scene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;

protected:
	void ControlsMenu();
};


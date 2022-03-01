#pragma once
#include "DefaultSettings.h"

class Game;
class PlayerController;

class Assignment1Scene : public fw::Scene
{
protected:
    PlayerController* m_pPlayerController = nullptr;

	std::vector<fw::GameObject*> m_meteors;
	std::vector<fw::GameObject*> m_debris;

	float m_meteorTimer = c_meteorSpawnDelay;
	float m_debrisTimer = c_debrisLifeSpan;

	bool m_spawnedDebris = false;
	bool m_canWalk = false;
	bool m_isPlaying = false;

	bool m_showStart = true;
	bool m_showWin = false;
	bool m_showDeath = false;

public:
	Assignment1Scene(Game* pGame);
    virtual ~Assignment1Scene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;

	void ReloadScene();

protected:
	fw::GameObject* CheckCollision(fw::CollisionEvent* pCollisionEvent, std::string nameOne, std::string nameTwo);

	void SpawnDebris(vec3 pos);

	void SetupPlatform();

	void FillDebrisPool();
	void FillMeteorPool();

	void ResetDebrisPool();
	void ResetMeteorPool();

	void ResetGame();

	void StartGameWindow();
	void DeathWindow();
	void WinWindow();


	void ControlsMenu();
};


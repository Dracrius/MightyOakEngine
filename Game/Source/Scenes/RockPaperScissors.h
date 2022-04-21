#pragma once

class Game;

class RockPaperScissors : public fw::Scene
{
protected:
    bool m_compChoose = false;
    bool m_playerChoose = false;
    float m_timeElapsed = 0.f;
    float m_reaction = 0.f;

    int m_playerPick = 0;
    int m_compPick = 0;

    bool m_showResults = false;

    fw::SpriteSheet* m_pSpriteSheet = nullptr;

public:
    RockPaperScissors(Game* pGame);
    virtual ~RockPaperScissors();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;

protected:
    void Results();
	void SettingsMenu();
};


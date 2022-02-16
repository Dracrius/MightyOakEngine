#pragma once

class Game;

class WaterScene : public fw::Scene
{
protected:
    float m_planeSize[2] = { 100.f, 100.f };
    int m_planeVertRes[2] = { 1000, 1000 };

	bool m_showSlider = false;
public:
    WaterScene(Game* pGame);
    virtual ~WaterScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;
};


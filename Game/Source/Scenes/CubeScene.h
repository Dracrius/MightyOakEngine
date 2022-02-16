#pragma once

class Game;

class CubeScene : public fw::Scene
{
protected:
	int m_rotationSpeed = 8;
	bool m_autoRotate = true;
	bool m_showSlider = false;

	float m_openRot[3] = { 0.f, 0.f, 0.f };
public:
    CubeScene(Game* pGame);
    virtual ~CubeScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;
};


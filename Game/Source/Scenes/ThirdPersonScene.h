#pragma once

class Game;
class PlayerController;

class ThirdPersonScene : public fw::Scene
{
protected:
	PlayerController* m_pPlayerController = nullptr;

    int m_rotationSpeed = 8;
    bool m_autoRotate = false;
	bool m_showOpenObj = false;
	std::string m_lastObj;
	bool m_hasTexture = true;
	std::string m_lastTexture;
	bool m_showSlider = false;

	char m_filename[260];
	char m_textureName[260];
	bool m_righthanded = true;

	float m_openPos[3];
	float m_openRot[3] = { 0.f, 0.f, 0.f };
	float m_openScale[3] = { 1.f, 1.f, 1.f };
public:
    ThirdPersonScene(Game* pGame);
    virtual ~ThirdPersonScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;

protected:
	void Slider(float& rot);
	void OpenObj();
	void OpenRecent();
	void SettingsMenu();
};


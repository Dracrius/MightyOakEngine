#pragma once

class Game;

class ObjScene : public fw::Scene
{
protected:
    int m_rotationSpeed = 8;
    bool m_autoRotate = true;
	bool m_showOpenObj = false;
	char* m_lastObj;
	bool m_hasTexture = true;
	char* m_lastTexture;
	bool m_showSlider = false;

	char m_filename[260];
	char m_textureName[260];
	bool m_righthanded = true;

	float m_openPos[3];
	float m_openRot[3] = { 0.f, 0.f, 0.f };
	float m_openScale[3] = { 1.f, 1.f, 1.f };
public:
    ObjScene(Game* pGame);
    virtual ~ObjScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;

protected:
	void Slider(float& rot);
	void OpenObj();
	void OpenRecent();
	void SettingsMenu();
};


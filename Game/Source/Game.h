#pragma once

#include "Framework.h"
#include "DefaultSettings.h"

class Game : public fw::GameCore
{
protected:
    fw::FWCore& m_FWCore;
    fw::ImGuiManager* m_pImGuiManager = nullptr;

    fw::FrameBufferObject* m_pOffScreenFBO = nullptr;

    std::map<std::string, fw::Scene*> m_Scenes;

    fw::Scene* m_pCurrentScene = nullptr;

	bool m_showDemo = false;
	bool m_showBGColorSelect = false;
	bool m_wireframeToggle = false;
	fw::Color4f m_backgroundColor = fw::Color4f::Black();
	fw::Color4f m_backupColor = c_defaultBackground;
public:
    Game(fw::FWCore& fwCore);
    Game(const Game& other) = delete; //Removes Copy Constructor
    virtual ~Game() override;

    void Init();
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

	virtual void SetCurrentScene(std::string scene);
	virtual void ResetBackgroundColor(bool toBlack);

    // Getters.
	fw::FWCore* GetFramework() { return &m_FWCore; }

protected:
	void BGColorSelect();
	void MainMenu();
	void HelpMarker(const char* desc);

};

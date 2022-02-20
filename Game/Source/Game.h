#pragma once

#include "Framework.h"

class Enemy;
class Player;
class PlayerController;

class Game : public fw::GameCore
{
protected:
    fw::FWCore& m_FWCore;
    fw::ImGuiManager* m_pImGuiManager = nullptr;


    std::map<std::string, fw::Mesh*> m_Meshes;
    std::map<std::string, fw::ShaderProgram*> m_Shaders;
    std::map<std::string, fw::Texture*> m_Textures;
    std::map<std::string, fw::Material*> m_Materials;
    std::map<std::string, fw::SpriteSheet*> m_SpriteSheets;
    std::map<std::string, fw::Scene*> m_Scenes;

    fw::Scene* m_pCurrentScene = nullptr;

	bool m_showDemo = false;
	bool m_showBGColorSelect = false;
	bool m_wireframeToggle = false;
	fw::Color4f m_backgroundColor = fw::Color4f(0.0f, 0.0f, 0.0f, 1.0f);
	fw::Color4f m_backupColor = fw::Color4f(0.0f, 0.0f, 0.2f, 1.0f);
public:
    Game(fw::FWCore& fwCore);
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
    fw::Mesh* GetMesh(std::string name) { return m_Meshes[name]; }
    fw::ShaderProgram* GetShader(std::string name) { return m_Shaders[name]; }
    fw::Texture* GetTexture(std::string name) { return m_Textures[name]; }
    fw::Material* GetMaterial(std::string name) { return m_Materials[name]; }
    fw::SpriteSheet* GetSpriteSheet(std::string name) { return m_SpriteSheets[name]; }

protected:
	void BGColorSelect();
	void MainMenu();
	void HelpMarker(const char* desc);

};

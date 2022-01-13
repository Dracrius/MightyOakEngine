#pragma once

#include "Framework.h"

class Enemy;
class Player;
class PlayerController;
class Tilemap;

class Game : public fw::GameCore
{
public:
    Game(fw::FWCore& fwCore);
    virtual ~Game() override;

    void Init();
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    // Getters.
    fw::Mesh* GetMesh(std::string name) { return m_Meshes[name]; }
    fw::ShaderProgram* GetShader(std::string name) { return m_Shaders[name]; }
    fw::Texture* GetTexture(std::string name) { return m_Textures[name]; }
    fw::SpriteSheet* GetSpriteSheet(std::string name) { return m_SpriteSheets[name]; }

protected:
    fw::FWCore& m_FWCore;
    fw::ImGuiManager* m_pImGuiManager = nullptr;

    PlayerController* m_pPlayerController = nullptr;

    std::map<std::string, fw::Mesh*> m_Meshes;
    std::map<std::string, fw::ShaderProgram*> m_Shaders;
    std::map<std::string, fw::Texture*> m_Textures;
    std::map<std::string, fw::SpriteSheet*> m_SpriteSheets;

    Tilemap* m_pTilemap = nullptr;

    fw::Camera* m_pCamera = nullptr;

    b2World* m_pPhysicsWorld;

    std::vector<fw::GameObject*> m_Objects;
};

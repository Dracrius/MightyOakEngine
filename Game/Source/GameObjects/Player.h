#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

struct spriteDirections
{
    std::vector<fw::SpriteSheet::SpriteInfo*> up;
    std::vector<fw::SpriteSheet::SpriteInfo*> down;
    std::vector<fw::SpriteSheet::SpriteInfo*> left;
    std::vector<fw::SpriteSheet::SpriteInfo*> right;

    spriteDirections(std::vector<fw::SpriteSheet::SpriteInfo*> upFrames, std::vector<fw::SpriteSheet::SpriteInfo*> downFrames, std::vector<fw::SpriteSheet::SpriteInfo*> leftFrames, std::vector<fw::SpriteSheet::SpriteInfo*> rightFrames) : up(upFrames), down(downFrames), left(leftFrames), right(rightFrames) {}
    spriteDirections() {}
};

class PlayerController;
class Tilemap;

class Player : public fw::GameObject
{
public:
    Player(fw::Scene* pScene, fw::Mesh* pMesh, fw::Material* pMaterial, vec2 pos, PlayerController* pController);
    virtual ~Player();

    virtual void Update(float deltaTime) override;
    //virtual void Draw() override;

    // Setters.
    void SetSpriteSheet(fw::SpriteSheet* pSpriteSheet) { m_pSpriteSheet = pSpriteSheet; SetAnimations(); }
    void SetTilemap(Tilemap* pTilemap) { m_pTilemap = pTilemap; }
    void SetAnimations();

    void CycleAnimFrames(int numFrames);

protected:
    PlayerController* m_pPlayerController = nullptr;

    spriteDirections m_playerDirections;

    int m_animFrame = 0;
    float m_timePassed = 0.f;
    float m_jumpTimer = 0.f;
    vec3 m_velocity;

    fw::SpriteSheet* m_pSpriteSheet = nullptr;
    Tilemap* m_pTilemap = nullptr;
};

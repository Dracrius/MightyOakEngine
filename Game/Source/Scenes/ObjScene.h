#pragma once

class Game;

class ObjScene : public fw::Scene
{
public:
    ObjScene(Game* pGame);
    virtual ~ObjScene();

    virtual void StartFrame(float deltaTime) override;

    virtual void OnEvent(fw::Event* pEvent) override;

    virtual void Update(float deltaTime) override;
};


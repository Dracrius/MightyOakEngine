#pragma once

namespace fw {

class Event;

class GameCore
{
public:
    virtual ~GameCore() = 0 {}

    virtual void StartFrame(float deltaTime) = 0;
    virtual void OnEvent(Event* pEvent) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
};

} // namespace fw

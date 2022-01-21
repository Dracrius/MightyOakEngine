#pragma once

#include "Math/Vector.h"

namespace fw {

class PhysicsBody;

class PhysicsWorld
{
protected:
    static const vec2 c_defaultGravity;

public:
    virtual ~PhysicsWorld() = 0 {}

    virtual void Update(float deltaTime) = 0;

    virtual void SetGravity(vec2 gravity) = 0;

    virtual PhysicsBody* CreateBody(bool isDynamic, vec2 size, float density, vec2 pos) = 0;
};

} // namespace fw

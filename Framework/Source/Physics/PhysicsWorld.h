#pragma once

#include "Math/Vector.h"

namespace fw {

class PhysicsBody;

class PhysicsWorld
{
protected:
    static const vec3 c_defaultGravity;

public:
    virtual ~PhysicsWorld() = 0 {}

    virtual void Update(float deltaTime) = 0;

    virtual void SetGravity(vec3 gravity) = 0;

    virtual PhysicsBody* CreateBody(bool isDynamic, vec3 size, float density, vec3 pos) = 0;
};

} // namespace fw

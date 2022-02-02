#pragma once

#include "Math/Vector.h"

namespace fw {

class PhysicsBody
{
public:
    virtual ~PhysicsBody() = 0 {}
    virtual vec3 GetPosition() = 0;
    virtual vec3 GetRotation() = 0;
};

} // namespace fw

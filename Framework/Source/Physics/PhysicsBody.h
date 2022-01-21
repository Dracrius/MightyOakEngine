#pragma once

#include "Math/Vector.h"

namespace fw {

class PhysicsBody
{
public:
    virtual ~PhysicsBody() = 0 {}
    virtual vec2 GetPosition() = 0;
};

} // namespace fw

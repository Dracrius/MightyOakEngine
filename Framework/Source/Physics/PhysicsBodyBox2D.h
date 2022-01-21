#pragma once
#include "Physics/PhysicsBody.h"
#include "Math/Vector.h"

namespace fw {

class PhysicsBodyBox2D : public PhysicsBody
{
protected:
    b2Body* m_pBody;
public:
    PhysicsBodyBox2D(b2Body* pBody);
    virtual ~PhysicsBodyBox2D();

    virtual vec2 GetPosition() override;
};

} // namespace fw

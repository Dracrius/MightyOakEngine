#pragma once

#include "Math/Vector.h"

namespace fw {

class PhysicsBody
{
public:
    virtual ~PhysicsBody() = 0 {}
    virtual vec3 GetPosition() = 0;
    virtual vec3 GetRotation() = 0;
    virtual vec3 GetVelocity() = 0;
    
    virtual void SetPosition(vec3 pos) = 0;
    virtual void SetTransform(vec3 pos, vec3 rot) = 0;
    virtual void SetVelocity(vec3 vel) = 0;

    virtual void ApplyForce(const vec3 &force, bool wake) = 0;
    virtual void ApplyForce(const vec3 &force, const vec3 point, bool wake) = 0;

    virtual void ApplyLinearImpulse(const vec3& impulse, bool wake) = 0;
    virtual void ApplyLinearImpulse(const vec3 &impulse, const vec3 point, bool wake) = 0;

    virtual void ApplyTorque(const vec3 &torque, bool wake) = 0;
};

} // namespace fw

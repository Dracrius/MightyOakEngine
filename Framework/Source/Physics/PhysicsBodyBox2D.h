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

    virtual vec3 GetPosition() override;
    virtual vec3 GetRotation() override;
    virtual vec3 GetVelocity() override;

    virtual void ApplyForce(const vec3& force, bool wake) override;
    virtual void ApplyForce(const vec3& force, const vec3 point, bool wake) override;

    virtual void ApplyLinearImpulse(const vec3& impulse, bool wake) override;
    virtual void ApplyLinearImpulse(const vec3& impulse, const vec3 point, bool wake) override;

    virtual void ApplyTorque(const vec3& torque, bool wake) override;
};

} // namespace fw

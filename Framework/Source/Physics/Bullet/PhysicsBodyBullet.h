#pragma once

#include "Physics/PhysicsBody.h"

class btRigidBody;

namespace fw {

class PhysicsWorldBullet;

class PhysicsBodyBullet : public PhysicsBody
{
public:
	PhysicsBodyBullet(PhysicsWorldBullet* pWorld, btRigidBody* pRigidBody);
	virtual ~PhysicsBodyBullet() override;

	// Getters.
	virtual vec3 GetPosition() override;
	virtual vec3 GetRotation() override;
	virtual vec3 GetVelocity() override;

	// Setters.
	virtual void SetPosition(vec3 pos) override;
	virtual void SetTransform(vec3 pos, vec3 rot) override;
    virtual void SetVelocity(vec3 vel) override;

    virtual void SetState(bool isEnabled) override;

	// Forces.
    virtual void ApplyForce(const vec3& force, bool wake) override;
    virtual void ApplyForce(const vec3& force, const vec3 point, bool wake) override;

    virtual void ApplyLinearImpulse(const vec3& impulse, bool wake) override;
    virtual void ApplyLinearImpulse(const vec3& impulse, const vec3 point, bool wake) override;

    virtual void ApplyTorque(const vec3& torque, bool wake) override;

	// Custom box2d methods.
	btRigidBody* GetbtBody() { return m_pBody; }

public:
	virtual void Editor_OutputBodyDetails() override;

protected:
	PhysicsWorldBullet* m_pWorld = nullptr;
	btRigidBody* m_pBody = nullptr;
};

} // namespace fw

#include "CoreHeaders.h"
#include "PhysicsBodyBox2D.h"
#include "../Libraries/imgui/imgui.h"

namespace fw {

PhysicsBodyBox2D::PhysicsBodyBox2D(b2Body* pBody)
{
    m_pBody = pBody;
}

PhysicsBodyBox2D::~PhysicsBodyBox2D()
{
}

vec3 PhysicsBodyBox2D::GetPosition()
{
    b2Vec2 pos = m_pBody->GetPosition();
    return vec3(pos.x, pos.y, 0.f);
}

vec3 PhysicsBodyBox2D::GetRotation()
{
    float rot = m_pBody->GetAngle();

    rot = -(rot / PI) * 180.f;

    return vec3(0.f, 0.f, rot);
}

vec3 PhysicsBodyBox2D::GetVelocity()
{
    b2Vec2 vel = m_pBody->GetLinearVelocity();

    return vec3(vel.x, vel.y, 0.f);
}

void PhysicsBodyBox2D::SetPosition(vec3 pos)
{
    float rot = m_pBody->GetAngle();
    m_pBody->SetTransform(b2Vec2(pos.x, pos.y), rot);
    m_pBody->SetAwake(true);
    m_pBody->SetLinearVelocity(b2Vec2());
    m_pBody->SetAngularVelocity(0.f);
}

void PhysicsBodyBox2D::SetTransform(vec3 pos, vec3 rot)
{
    rot = -(rot / PI) * 180.f;

    m_pBody->SetTransform(b2Vec2(pos.x, pos.y), rot.z);
    m_pBody->SetAwake(true);
    m_pBody->SetLinearVelocity(b2Vec2());
    m_pBody->SetAngularVelocity(0.f);
}


void PhysicsBodyBox2D::SetVelocity(vec3 vel)
{
    m_pBody->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void PhysicsBodyBox2D::ApplyForce(const vec3& force, bool wake)
{
    b2Vec2 twoDForce = b2Vec2(force.x, force.y);

    m_pBody->ApplyForceToCenter(twoDForce, wake);
}

void PhysicsBodyBox2D::ApplyForce(const vec3& force, const vec3 point, bool wake)
{
    b2Vec2 twoDForce = b2Vec2(force.x, force.y);
    b2Vec2 twoDPoint = b2Vec2(point.x, point.y);

    m_pBody->ApplyForce(twoDForce, twoDPoint, wake);
}

void PhysicsBodyBox2D::ApplyLinearImpulse(const vec3& impulse, bool wake)
{
    b2Vec2 twoDImpulse = b2Vec2(impulse.x, impulse.y);

    m_pBody->ApplyLinearImpulseToCenter(twoDImpulse, wake);
}

void PhysicsBodyBox2D::ApplyLinearImpulse(const vec3& impulse, const vec3 point, bool wake)
{
    b2Vec2 twoDImpulse = b2Vec2(impulse.x, impulse.y);
    b2Vec2 twoDPoint = b2Vec2(point.x, point.y);

    m_pBody->ApplyLinearImpulse(twoDImpulse, twoDPoint, wake);
}

void PhysicsBodyBox2D::ApplyTorque(const vec3& torque, bool wake)
{
    m_pBody->ApplyTorque(torque.z, wake);
}

void PhysicsBodyBox2D::Editor_OutputBodyDetails()
{
	bool fixed = m_pBody->IsFixedRotation();

	if (ImGui::Checkbox("Fixed Rotation", &fixed))
	{
		m_pBody->SetFixedRotation(fixed);
	}
}

} // namespace fw

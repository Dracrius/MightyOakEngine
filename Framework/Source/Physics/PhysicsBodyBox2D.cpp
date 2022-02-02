#include "CoreHeaders.h"
#include "PhysicsBodyBox2D.h"

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
    return vec3(rot, 0.f, 0.f);
}

} // namespace fw

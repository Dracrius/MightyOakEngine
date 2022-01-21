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

vec2 PhysicsBodyBox2D::GetPosition()
{
    b2Vec2 pos = m_pBody->GetPosition();
    return vec2(pos.x, pos.y);
}

} // namespace fw

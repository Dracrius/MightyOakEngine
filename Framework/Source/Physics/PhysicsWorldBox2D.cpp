#include "CoreHeaders.h"
#include "PhysicsWorldBox2D.h"
#include "PhysicsBodyBox2D.h"

namespace fw {

PhysicsWorldBox2D::PhysicsWorldBox2D()
{
	m_pWorld = new b2World(c_defaultGravity);
}

PhysicsWorldBox2D::~PhysicsWorldBox2D()
{
	delete m_pWorld;
}

void PhysicsWorldBox2D::Update(float deltaTime)
{
	m_pWorld->Step(deltaTime, 8, 3);
}

void PhysicsWorldBox2D::SetGravity(vec3 gravity)
{
	m_pWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

PhysicsBody* PhysicsWorldBox2D::CreateBody(bool isDynamic, vec2 size, float density, vec3 pos)
{
    b2BodyDef bodydef;
    bodydef.position = b2Vec2(pos.x, pos.y);

    if (isDynamic)
    {
        bodydef.type = b2_dynamicBody;
    }
    else
    {
        bodydef.type = b2_staticBody;
    }

    bodydef.userData.pointer = reinterpret_cast<uintptr_t>(this);

    b2PolygonShape shape;
    shape.SetAsBox(size.x / 2, size.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;

    b2Body* body = m_pWorld->CreateBody(&bodydef);
    body->CreateFixture(&fixtureDef);

    PhysicsBody* physicsBody = new PhysicsBodyBox2D(body);

    return physicsBody;
}

} // namespace fw

#include "CoreHeaders.h"
#include "PhysicsWorldBox2D.h"
#include "PhysicsBodyBox2D.h"
#include "Objects/GameObject.h"
#include "Events/EventManager.h"
#include "Events/Event.h"
#include "DebugDrawBox2D.h"

namespace fw {

void ContactListenerBox2D::BeginContact(b2Contact* contact)
{
	b2Fixture* pFixtureA = contact->GetFixtureA();
	b2Fixture* pFixtureB = contact->GetFixtureB();

	b2Body* pBodyA = pFixtureA->GetBody();
	b2Body* pBodyB = pFixtureB->GetBody();

	GameObject* pObjectA = reinterpret_cast<GameObject*>(pBodyA->GetUserData().pointer);
	GameObject* pObjectB = reinterpret_cast<GameObject*>(pBodyB->GetUserData().pointer);

	CollisionEvent* pCollisionEvent = new CollisionEvent(pObjectA, pObjectB, ContactState::Begin);
	m_pEventManager->AddEvent(pCollisionEvent);
}

void ContactListenerBox2D::EndContact(b2Contact* contact)
{
}

void ContactListenerBox2D::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void ContactListenerBox2D::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}

PhysicsWorldBox2D::PhysicsWorldBox2D(EventManager* pEventManager) : PhysicsWorld(pEventManager)
{
	m_pWorld = new b2World(c_defaultGravity);

	m_pContactListener = new ContactListenerBox2D(pEventManager);
	m_pDebugDraw = new DebugDrawBox2D();

	m_pWorld->SetContactListener(m_pContactListener);
	m_pDebugDraw->SetFlags(b2Draw::e_shapeBit);
	m_pWorld->SetDebugDraw(m_pDebugDraw);

	b2BodyDef bodyDef;
	m_pGroundBody = m_pWorld->CreateBody(&bodyDef);
}

PhysicsWorldBox2D::~PhysicsWorldBox2D()
{
	delete m_pDebugDraw;
	delete m_pContactListener;
	delete m_pWorld;
}

void PhysicsWorldBox2D::Update(float deltaTime)
{
	float timeStep = 1 / 60.0f;

	bool didAWorldStep = false;
	m_timeAccumulated += deltaTime;

	while (m_timeAccumulated >= timeStep)
	{
		didAWorldStep = true;
		m_pWorld->Step(deltaTime, 8, 3);
		m_timeAccumulated -= timeStep;
	}

	if (didAWorldStep == false)
	{
		m_pWorld->ClearForces();
	}
}

void PhysicsWorldBox2D::DebugDraw(Camera* pCamera, Material* pMaterial)
{
	m_pWorld->DebugDraw();
	m_pDebugDraw->Draw(pCamera, pMaterial);
}

void PhysicsWorldBox2D::SetGravity(vec3 gravity)
{
	m_pWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

PhysicsBody* PhysicsWorldBox2D::CreateBody(GameObject* owner, bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot)
{
    b2BodyDef bodydef;
    bodydef.position = b2Vec2(pos.x, pos.y);
    bodydef.angle = rot.z;

    if (isDynamic)
    {
        bodydef.type = b2_dynamicBody;
    }
    else
    {
        bodydef.type = b2_staticBody;
    }

    bodydef.userData.pointer = reinterpret_cast<uintptr_t>(owner);

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

PhysicsBody* PhysicsWorldBox2D::CreateBody(GameObject* owner, bool isDynamic, float radius, float density, vec3 pos, vec3 rot)
{
	b2BodyDef bodydef;
	bodydef.position = b2Vec2(pos.x, pos.y);
	bodydef.angle = rot.z;

	if (isDynamic)
	{
		bodydef.type = b2_dynamicBody;
	}
	else
	{
		bodydef.type = b2_staticBody;
	}

	bodydef.userData.pointer = reinterpret_cast<uintptr_t>(owner);

	b2CircleShape shape;
	shape.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;

	b2Body* body = m_pWorld->CreateBody(&bodydef);
	body->CreateFixture(&fixtureDef);

	PhysicsBody* physicsBody = new PhysicsBodyBox2D(body);

	return physicsBody;
}

PhysicsBody* PhysicsWorldBox2D::CreateBody(GameObject* owner, bool isDynamic, float density, TransformComponent* pTransform)
{
    return CreateBody(owner, isDynamic, pTransform->GetScale(), density, pTransform->GetPosition(), pTransform->GetRotation());
}

void PhysicsWorldBox2D::CreateJoint(PhysicsBody* pBody, vec3 pos)
{
	b2RevoluteJointDef jointDef;

	jointDef.Initialize(static_cast<PhysicsBodyBox2D*>(pBody)->Getb2Body(), m_pGroundBody, pos);
	jointDef.enableMotor = true;
	jointDef.motorSpeed = 10.0f;
	jointDef.maxMotorTorque = 10.0;

	b2Joint* pJoint = m_pWorld->CreateJoint(&jointDef);
}

void PhysicsWorldBox2D::CreateSlider(PhysicsBody* pBody, vec3 pos)
{
	assert(false); // implement this.
}

void PhysicsWorldBox2D::CreateSensor(GameObject* owner, TransformComponent* pTransform)
{
    assert(false); // implement this.
}

} // namespace fw

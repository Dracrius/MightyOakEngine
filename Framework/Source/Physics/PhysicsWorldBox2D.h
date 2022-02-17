#pragma once

#include "../Libraries/box2d/include/box2d/box2d.h"
#include "Math/Vector.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsBody.h"

namespace fw {

class EventManager;
class GameObject;

class ContactListenerBox2D : public b2ContactListener
{
protected:
	EventManager* m_pEventManager = nullptr;
public:
	ContactListenerBox2D(EventManager* pEventManager) : m_pEventManager(pEventManager){}
	virtual ~ContactListenerBox2D() {}

	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};

class PhysicsWorldBox2D : public PhysicsWorld
{
protected:
    b2World* m_pWorld = nullptr;
	b2ContactListener* m_pContactListener = nullptr;

public:
    PhysicsWorldBox2D(EventManager* pEventManager);
    virtual ~PhysicsWorldBox2D();

    virtual void Update(float deltaTime) override;

    virtual void SetGravity(vec3 gravity) override;
    
    virtual PhysicsBody* CreateBody(GameObject* owner, bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot) override;

    b2World* Getb2World() { return m_pWorld; }
};

} // namespace fw

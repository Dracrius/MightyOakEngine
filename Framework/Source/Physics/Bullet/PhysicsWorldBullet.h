#pragma once

#include "Physics/PhysicsWorld.h"

class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionShape;
class btConstraintSolver;
class btDispatcher;
class btDynamicsWorld;

namespace fw {

class EventManager;
class PhysicsBodyBullet;

class PhysicsWorldBullet : public PhysicsWorld
{
    friend PhysicsBodyBullet;

public:
    PhysicsWorldBullet(EventManager* pEventManager);
    virtual ~PhysicsWorldBullet();

    virtual void Update(float deltaTime) override;

    virtual void DebugDraw(Camera* pCamera, Material* pMaterial);

    virtual void SetGravity(vec3 gravity) override;

    virtual PhysicsBody* CreateBody(GameObject* owner, bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot) override;
    virtual PhysicsBody* CreateBody(GameObject* owner, bool isDynamic, float radius, float density, vec3 pos, vec3 rot) override;
    virtual PhysicsBody* CreateBody(GameObject* owner, bool isDynamic, float density, TransformComponent* pTransform) override;

    virtual void CreateJoint(PhysicsBody* pBody, vec3 pos) override;

    virtual void CreateSlider(PhysicsBody* pBody, vec3 pos) override;

    // Getters
    btDynamicsWorld* GetbtWorld() { return m_pWorld; };

protected:
    btDynamicsWorld* m_pWorld = nullptr;

    btCollisionConfiguration* m_pCollisionConfiguration = nullptr;
    btDispatcher* m_pDispatcher = nullptr;
    btBroadphaseInterface* m_pBroadphase = nullptr;
    btConstraintSolver* m_pConstraintSolver = nullptr;
};

} // namespace fw
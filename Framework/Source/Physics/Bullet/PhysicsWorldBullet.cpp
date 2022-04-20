#include "CoreHeaders.h"

#include "PhysicsWorldBullet.h"
#include "PhysicsBodyBullet.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "Components/TransformComponent.h"

#include "btBulletDynamicsCommon.h"

namespace fw {


PhysicsWorldBullet::PhysicsWorldBullet(EventManager* pEventManager) : PhysicsWorld(pEventManager)
{
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher( m_pCollisionConfiguration );
    m_pBroadphase = new btDbvtBroadphase();
    m_pConstraintSolver = new btSequentialImpulseConstraintSolver;

    m_pWorld = new btDiscreteDynamicsWorld( m_pDispatcher, m_pBroadphase, m_pConstraintSolver, m_pCollisionConfiguration );
    m_pWorld->setGravity( btVector3(c_defaultGravity.x, c_defaultGravity.y, c_defaultGravity.z) );
}

PhysicsWorldBullet::~PhysicsWorldBullet()
{
    delete m_pWorld;

    delete m_pConstraintSolver;
    delete m_pBroadphase;
    delete m_pDispatcher;
    delete m_pCollisionConfiguration;
}

void PhysicsWorldBullet::Update(float deltaTime)
{
    m_pWorld->stepSimulation( deltaTime, 0 );
}

void PhysicsWorldBullet::DebugDraw(Camera* pCamera, Material* pMaterial)
{
}

void PhysicsWorldBullet::SetGravity(vec3 gravity)
{
    m_pWorld->setGravity( btVector3(gravity.x, gravity.y, gravity.z) );
}

PhysicsBody* PhysicsWorldBullet::CreateBody(GameObject* owner, bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot)
{
    // Setup the position and orientation transform for the object.
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
    transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

    // Create the shape, ideally these would be shared between objects.
    vec3 scale = size;
    btCollisionShape* pShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z) / 2);

    // Density/mass of 0 means the object is static.
    assert(isDynamic || density == 0.0f);

    // For dynamic objects, calculate the center of inertia based on the mass/density and shape.
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
    {
        pShape->calculateLocalInertia(density, localInertia);
    }

    // Create a motion state object, this can provide callbacks for syncing the physics object to our transform,
    // but would require us overriding this class.
    btMotionState* pMotionState = new btDefaultMotionState(transform);

    // Create the rigid body.
    btRigidBody::btRigidBodyConstructionInfo rbInfo(density, pMotionState, pShape, localInertia);
    btRigidBody* pRigidBody = new btRigidBody(rbInfo);
    pRigidBody->setUserPointer(owner);

    // Add it to the world.
    m_pWorld->addRigidBody(pRigidBody);

    // Wrap it in our physic body class and return it.
    PhysicsBodyBullet* pBody = new PhysicsBodyBullet(this, pRigidBody);
    return pBody;
}

PhysicsBody* PhysicsWorldBullet::CreateBody(GameObject* owner, bool isDynamic, float radius, float density, vec3 pos, vec3 rot)
{
    assert(false); // implement this.
    return nullptr;
}

PhysicsBody* PhysicsWorldBullet::CreateBody(GameObject* owner, bool isDynamic, float density, TransformComponent* pTransform)
{
    vec3 pos = pTransform->GetPosition();
    vec3 rot = pTransform->GetRotation();
    vec3 size = pTransform->GetScale();

    return CreateBody(owner, isDynamic, size, density, pos, rot);
}

void PhysicsWorldBullet::CreateJoint(PhysicsBody* pBody, vec3 pos)
{
    //assert( false ); // implement this.

    btRigidBody* bodyA = static_cast<PhysicsBodyBullet*>(pBody)->GetbtBody();
    btVector3 btPos = btVector3(pos.x, pos.y, pos.z);

    btTypedConstraint* p2p = new btPoint2PointConstraint(*bodyA, btPos);
    m_pWorld->addConstraint(p2p);
}

void PhysicsWorldBullet::CreateSlider(PhysicsBody* pBody, vec3 pos)
{
    //assert( false ); // implement this.

    btRigidBody* bodyA = static_cast<PhysicsBodyBullet*>(pBody)->GetbtBody();
    btVector3 btPos = btVector3(pos.x, pos.y, pos.z);
    btTransform frameInA;
    frameInA = btTransform::getIdentity();
    frameInA.setOrigin(btPos);

    btTypedConstraint* slider = new btSliderConstraint(*bodyA, frameInA,true);
    static_cast<btSliderConstraint*>(slider)->setLowerLinLimit(btScalar(-pos.x));
    static_cast<btSliderConstraint*>(slider)->setUpperLinLimit(btScalar(pos.x));
    m_pWorld->addConstraint(slider);
}

} // namespace fw


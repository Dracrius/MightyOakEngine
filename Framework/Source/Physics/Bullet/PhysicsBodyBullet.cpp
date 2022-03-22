#include "CoreHeaders.h"

#include "PhysicsBodyBullet.h"
#include "PhysicsWorldBullet.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

#include "btBulletDynamicsCommon.h"

namespace fw {

PhysicsBodyBullet::PhysicsBodyBullet(PhysicsWorldBullet* pWorld, btRigidBody* pRigidBody)
{
    m_pWorld = pWorld;
    m_pBody = pRigidBody;
}

PhysicsBodyBullet::~PhysicsBodyBullet()
{
    m_pWorld->m_pWorld->removeRigidBody( m_pBody );
    delete m_pBody->getMotionState();
    delete m_pBody->getCollisionShape();
    delete m_pBody;
}

vec3 PhysicsBodyBullet::GetPosition()
{
    btVector3 pos = m_pBody->getWorldTransform().getOrigin();
    return vec3( pos.x(), pos.y(), pos.z() );
}

vec3 PhysicsBodyBullet::GetRotation()
{
    // Grab the bullet transform.
    btTransform transform = m_pBody->getWorldTransform();

    // Convert it to a transform matrix.
    matrix matrix;
    transform.getOpenGLMatrix( &matrix.m11 );

    // Convert it to angles.
    vec3 angles = matrix.GetEulerAngles();
    return angles;
}

vec3 PhysicsBodyBullet::GetVelocity()
{
    return vec3();
}

void PhysicsBodyBullet::SetPosition(vec3 pos)
{
}

void PhysicsBodyBullet::SetTransform(vec3 pos, vec3 rot)
{
}

void PhysicsBodyBullet::SetVelocity(vec3 vel)
{
}

void PhysicsBodyBullet::SetState(bool isEnabled)
{
}

void PhysicsBodyBullet::ApplyForce(const vec3& force, bool wake)
{
    m_pBody->applyCentralForce( btVector3(force.x, force.y, force.z) );
    m_pBody->activate( wake );
}

void PhysicsBodyBullet::ApplyForce(const vec3& force, const vec3 point, bool wake)
{
}

void PhysicsBodyBullet::ApplyLinearImpulse(const vec3& impulse, bool wake)
{
}

void PhysicsBodyBullet::ApplyLinearImpulse(const vec3& impulse, const vec3 point, bool wake)
{
}

void PhysicsBodyBullet::ApplyTorque(const vec3& torque, bool wake)
{
    m_pBody->applyTorque(btVector3(torque.x, torque.y, torque.z));
    m_pBody->activate(wake);
}

void PhysicsBodyBullet::Editor_OutputBodyDetails()
{
}

} // namespace fw

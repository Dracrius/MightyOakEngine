#pragma once

#include "Component.h"
#include "Math/Vector.h"

namespace fw {

class PhysicsBody;
class PhysicsWorld;

class PhysicsBodyComponent: public Component
{
protected:
    PhysicsBody* m_pPhysicsBody = nullptr;

public:
    PhysicsBodyComponent();
    virtual ~PhysicsBodyComponent();

    static const char* GetStaticType() { return "PhysicsBodyComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }

    void Update(float deltaTime);

    virtual void CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density);
    virtual void CreateBody(PhysicsWorld* pWorld, bool isDynamic, float radius, float density);
    virtual void CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density);

    PhysicsBody* GetPhysicsBody() { return m_pPhysicsBody; }

    virtual void ApplyImpulse(const vec3& impulse);
    virtual void ApplyTorque(const vec3& torque);
};

} // namespace fw

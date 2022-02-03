#pragma once

#include "../Libraries/box2d/include/box2d/box2d.h"
#include "Math/Vector.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsBody.h"

namespace fw {

class PhysicsWorldBox2D : public PhysicsWorld
{
protected:
    b2World* m_pWorld;

public:
    PhysicsWorldBox2D();
    virtual ~PhysicsWorldBox2D();

    virtual void Update(float deltaTime) override;

    virtual void SetGravity(vec3 gravity) override;
    
    virtual PhysicsBody* CreateBody(bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot) override;

    b2World* Getb2World() { return m_pWorld; }
};

} // namespace fw

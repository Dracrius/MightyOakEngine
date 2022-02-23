#pragma once

#include "Math/Vector.h"

namespace fw {

class PhysicsBody;
class EventManager;
class GameObject;
class Camera;
class Material;

class PhysicsWorld
{
protected:
    static const vec3 c_defaultGravity;
	EventManager* m_pEventManager = nullptr;

public:
	PhysicsWorld(EventManager* pEventManager) : m_pEventManager(pEventManager) {}
    virtual ~PhysicsWorld() = 0 {}

    virtual void Update(float deltaTime) = 0;

	virtual void DebugDraw(Camera* pCamera, Material* pMaterial) = 0;

    virtual void SetGravity(vec3 gravity) = 0;

    virtual PhysicsBody* CreateBody(GameObject* owner, bool isDynamic, vec3 size, float density, vec3 pos, vec3 rot) = 0;
};

} // namespace fw

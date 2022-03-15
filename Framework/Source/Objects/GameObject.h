#pragma once

#include "..\Libraries\box2d\include\box2d\box2d.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Components/TransformComponent.h"

namespace fw {

class Camera;
class Component;
class MeshComponent;
class GameCore;
class Mesh;
class Material;
class PhysicsWorld;
class PhysicsBody;
class Scene;

class GameObject
{
protected:
	std::string m_name;
    Scene* m_pScene = nullptr;

    //Physics Comp
    PhysicsBody* m_pPhysicsBody = nullptr;

	TransformComponent* m_pTramsform = nullptr;
    std::vector<Component*> m_pComponents;

	bool m_enabled = true;

public:
    GameObject(Scene* pScene, vec3 pos, vec3 rot);
    virtual ~GameObject();

    virtual void Update(float deltaTime);

	void SetState(bool isEnabled);

	virtual void CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density);
	virtual void CreateBody(PhysicsWorld* pWorld, bool isDynamic, float radius, float density);
	virtual void CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density);

    void AddComponent(Component* pComponent);
	void AddCompFromManager(Component* pComponent);
    void RemoveCompFromManager(Component* pComponent);
	Component* GetComponent(const char* component);

	template <class Type> Type* GetComponent()
	{
		for (Component* pComponent : m_pComponents)
		{
			if (pComponent->GetType() == Type::GetStaticType())
			{
				return static_cast<Type*>(pComponent);
			}
		}

		return nullptr;
	}

    // Getters.
	TransformComponent* GetTransform() { return m_pTramsform; }

	std::string GetName() { return m_name; }

    vec3 GetPosition() { return m_pTramsform->GetPosition(); }
    vec3 GetRotation() { return  m_pTramsform->GetRotation(); }
    vec3 GetScale() { return  m_pTramsform->GetScale(); }

    // Setters.
    //void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

	void SetName(std::string name) { m_name = name; }

	void SetPosition(vec3 pos);
	void SetRotation(vec3 rot);
    void SetScale(vec3 scale) { m_pTramsform->SetScale(scale); }

	virtual void ApplyImpulse(const vec3& impulse);
	virtual void ApplyTorque(const vec3& torque);

	void Editor_OutputObjectDetails();

};

} // namespace fw

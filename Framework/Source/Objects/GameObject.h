#pragma once

#include "..\Libraries\box2d\include\box2d\box2d.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

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
public:
    GameObject(Scene* pScene, vec3 pos, vec3 rot);
    virtual ~GameObject();

    virtual void Update(float deltaTime);

    void CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density);
    void CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density);

    void AddComponent(Component* pComponent);
    void RemoveComponent(Component* pComponent);
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
    const matrix& GetWorldTransform();
    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetScale() { return m_Scale; }

    // Setters.
    //void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

    void SetPosition(vec3 pos) { m_Position = pos; }
    void SetRotation(vec3 rot) { m_Rotation = rot; }
    void SetScale(vec3 scale) { m_Scale = scale; }

protected:
    Scene* m_pScene = nullptr;

    //Physics Comp
    PhysicsBody* m_pPhysicsBody = nullptr;

    //MeshComponent* m_pMeshComponent = nullptr;
    std::vector<Component*> m_pComponents;

    //Transform Comp
    matrix m_WorldTransform;
    vec3 m_Position = vec3( 0, 0, 0 );
    vec3 m_Rotation = vec3(0, 0, 0);
    vec3 m_Scale = vec3(1, 1, 1);
};

} // namespace fw

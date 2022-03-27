#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Components/TransformComponent.h"

namespace fw {

class Component;
class Scene;

class GameObject
{
protected:
	std::string m_name;
    Scene* m_pScene = nullptr;

	TransformComponent* m_pTransform = nullptr;
    std::vector<Component*> m_pComponents;

	bool m_enabled = true;

public:
    GameObject(Scene* pScene, vec3 pos, vec3 rot);
    virtual ~GameObject();

	void SetState(bool isEnabled);

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
	TransformComponent* GetTransform() { return m_pTransform; }

	std::string GetName() { return m_name; }

    bool GetState() { return m_enabled; }

    vec3 GetPosition() { return m_pTransform->GetPosition(); }
    vec3 GetRotation() { return  m_pTransform->GetRotation(); }
    vec3 GetScale() { return  m_pTransform->GetScale(); }

    Scene* GetScene() { return m_pScene; }

    // Setters.
	void SetName(std::string name) { m_name = name; }

	void SetPosition(vec3 pos);
	void SetRotation(vec3 rot);
    void SetScale(vec3 scale) { m_pTransform->SetScale(scale); }

	void Editor_OutputObjectDetails();
};

} // namespace fw

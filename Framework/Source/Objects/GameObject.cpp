#include "CoreHeaders.h"

#include "../Libraries/imgui/imgui.h"
#include "GameObject.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/LightComponent.h"
#include "Components/PhysicsBodyComponent.h"
#include "Components/ComponentManager.h"
#include "Physics/PhysicsBody.h"
#include "Scene.h"

namespace fw {

GameObject::GameObject(Scene* pScene, vec3 pos, vec3 rot)
    : m_pScene(pScene)
{
	m_pTransform = new TransformComponent(pos, rot, vec3(1));
	AddComponent(m_pTransform);
}

GameObject::~GameObject()
{
    for (auto pComponent : m_pComponents)
    {
        if (pComponent != nullptr)
        {
			if (pComponent->GetType() == MeshComponent::GetStaticType())
			{
				if (m_enabled)
				{
					m_pScene->GetComponentManager()->RemoveComponent(pComponent);
				}
			}
			else
			{
				m_pScene->GetComponentManager()->RemoveComponent(pComponent);
			}
            delete pComponent;
        }
    }
}

void GameObject::SetState(bool isEnabled)
{
	if (isEnabled != m_enabled)
	{
        PhysicsBodyComponent* pPhysicsBody = GetComponent<fw::PhysicsBodyComponent>();

		if (pPhysicsBody)
		{
            pPhysicsBody->GetPhysicsBody()->SetState(isEnabled);
		}

		MeshComponent* pMesh = GetComponent<fw::MeshComponent>();

		if (pMesh)
		{
			if (isEnabled)
			{
				AddCompFromManager(pMesh);
			}
			else
			{
				RemoveCompFromManager(pMesh);
			}
		}
	}

	m_enabled = isEnabled;
}

void GameObject::AddComponent(Component* pComponent)
{
    pComponent->SetGameObject(this);
	if (m_enabled)
	{
		m_pScene->GetComponentManager()->AddComponent(pComponent);
	}

    m_pComponents.push_back(pComponent);
}

void GameObject::AddCompFromManager(Component* pComponent)
{
	m_pScene->GetComponentManager()->AddComponent(pComponent);
}

void GameObject::RemoveCompFromManager(Component* pComponent)
{
	m_pScene->GetComponentManager()->RemoveComponent(pComponent);
}

Component* GameObject::GetComponent(const char* component)
{
	for (auto pComponent : m_pComponents)
	{
		if (pComponent->GetType() == component)
		{
			return pComponent;
		}
	}
    return nullptr;
}

void GameObject::SetPosition(vec3 pos)
{
	m_pTransform->SetPosition(pos);

    PhysicsBodyComponent* pPhysicsBody = GetComponent<fw::PhysicsBodyComponent>();

    if (pPhysicsBody)
    {
        pPhysicsBody->GetPhysicsBody()->SetPosition(pos);
    }
}

void GameObject::SetRotation(vec3 rot)
{
	m_pTransform->SetRotation(rot);

    PhysicsBodyComponent* pPhysicsBody = GetComponent<fw::PhysicsBodyComponent>();

    if (pPhysicsBody)
    {
        pPhysicsBody->GetPhysicsBody()->SetTransform(m_pTransform->GetPosition(), rot);
    }
}

void GameObject::Editor_OutputObjectDetails()
{
	vec3 pos = m_pTransform->GetPosition();
	vec3 rot = m_pTransform->GetRotation();
	vec3 scale = m_pTransform->GetScale();
	bool hasPhysBody = GetComponent<fw::PhysicsBodyComponent>() ? true : false;
	bool isLight = GetComponent<fw::LightComponent>() ? true : false;

	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Separator();
	ImGui::DragFloat3("Position", &pos.x, 0.01f);
	if (hasPhysBody)
	{
		ImGui::DragFloat("Rotation", &rot.z, 0.01f);
		//ImGui::DragFloat2("Scale", &scale.x, 0.01f; //Save For Experimenting
	}
	else
	{
		ImGui::DragFloat3("Rotation", &rot.x, 0.01f);
		ImGui::DragFloat2("Scale", &scale.x, 0.01f);
	}
	ImGui::Separator();
	if (isLight)
	{
		Color4f lightColor = GetComponent<fw::LightComponent>()->GetDetails().diffuse;
		vec3 sliderColor = vec3(lightColor.r, lightColor.g, lightColor.b);
		ImGui::DragFloat3("Colour", &sliderColor.x, 0.01f);

		if (sliderColor != vec3(lightColor.r, lightColor.g, lightColor.b))
		{
			GetComponent<fw::LightComponent>()->SetDiffuse(Color4f(sliderColor.x, sliderColor.y, sliderColor.z, 1.f));
		}
	}


	ImGui::Checkbox("Has Physics Body", &hasPhysBody);

    PhysicsBodyComponent* pPhysicsBody = GetComponent<fw::PhysicsBodyComponent>();

	if (pPhysicsBody)
	{
        pPhysicsBody->GetPhysicsBody()->Editor_OutputBodyDetails();

		if (pos != m_pTransform->GetPosition())
		{
            pPhysicsBody->GetPhysicsBody()->SetPosition(pos);
		}
		if (rot != m_pTransform->GetRotation())
		{
            pPhysicsBody->GetPhysicsBody()->SetTransform(pos, rot);
		}
	}
	else
	{
		m_pTransform->SetPosition(pos);
		m_pTransform->SetRotation(rot);
		m_pTransform->SetScale(scale);
	}

}

} // namespace fw

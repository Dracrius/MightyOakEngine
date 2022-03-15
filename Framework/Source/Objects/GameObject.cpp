#include "Framework.h"

#include "GameObject.h"
#include "Math/Matrix.h"
#include "Material.h"

namespace fw {

GameObject::GameObject(Scene* pScene, vec3 pos, vec3 rot)
    : m_pScene(pScene)
{
	m_pTramsform = new TransformComponent(pos, rot, vec3(1));
	AddComponent(m_pTramsform);
}

GameObject::~GameObject()
{
    delete m_pPhysicsBody;

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

void GameObject::Update(float deltaTime)
{
    if (m_pPhysicsBody)
    {
		m_pTramsform->SetPosition(m_pPhysicsBody->GetPosition());
		m_pTramsform->SetRotation(m_pPhysicsBody->GetRotation());
    }
}

void GameObject::SetState(bool isEnabled)
{
	if (isEnabled != m_enabled)
	{
		if (m_pPhysicsBody)
		{
			m_pPhysicsBody->SetState(isEnabled);
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

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density)
{
    CreateBody(pWorld, isDynamic, m_pTramsform->GetScale(), density);
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(this, isDynamic, size, density, m_pTramsform->GetPosition(), m_pTramsform->GetRotation());

	if (!m_enabled)
	{
		m_pPhysicsBody->SetState(false);
	}
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float radius, float density)
{
	m_pPhysicsBody = pWorld->CreateBody(this, isDynamic, radius, density, m_pTramsform->GetPosition(), m_pTramsform->GetRotation());

	if (!m_enabled)
	{
		m_pPhysicsBody->SetState(false);
	}
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
	m_pTramsform->SetPosition(pos);
	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->SetPosition(pos);
	}
}

void GameObject::SetRotation(vec3 rot)
{
	m_pTramsform->SetRotation(rot);
	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->SetTransform(m_pTramsform->GetPosition(), rot);
	}
}

void GameObject::ApplyImpulse(const vec3& impulse)
{
	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->ApplyLinearImpulse(impulse, true);
	}
}

void GameObject::ApplyTorque(const vec3& torque)
{
	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->ApplyTorque(torque, true);
	}
}

void GameObject::Editor_OutputObjectDetails()
{
	vec3 pos = m_pTramsform->GetPosition();
	vec3 rot = m_pTramsform->GetRotation();
	vec3 scale = m_pTramsform->GetScale();
	bool hasPhysBody = m_pPhysicsBody ? true : false;

	ImGui::Text("Name: %s", m_name.c_str());
	ImGui::Separator();
	ImGui::DragFloat3("Position", &pos.x, 0.01f);
	if (hasPhysBody)
	{
		ImGui::DragFloat("Rotation", &rot.z, 0.01f);
		//ImGui::DragFloat2("Scale", &scale.x, 0.01f;
	}
	else
	{
		ImGui::DragFloat3("Rotation", &rot.x, 0.01f);
		ImGui::DragFloat2("Scale", &scale.x, 0.01f);
	}
	ImGui::Separator();


	ImGui::Checkbox("Has Physics Body", &hasPhysBody);

	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->Editor_OutputBodyDetails();

		if (pos != m_pTramsform->GetPosition())
		{
			m_pPhysicsBody->SetPosition(pos);
		}
		if (rot != m_pTramsform->GetRotation())
		{
			m_pPhysicsBody->SetTransform(pos, rot);
		}
	}
	else
	{
		m_pTramsform->SetPosition(pos);
		m_pTramsform->SetRotation(rot);
		m_pTramsform->SetScale(scale);
	}

}

} // namespace fw

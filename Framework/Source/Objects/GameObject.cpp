#include "Framework.h"

#include "GameObject.h"
#include "Math/Matrix.h"
#include "Material.h"
#include "Components/MeshComponent.h"

namespace fw {

GameObject::GameObject(Scene* pScene, vec3 pos, vec3 rot)
    : m_pScene(pScene)
    , m_Position( pos )
    , m_Rotation( rot )
{
    //if (pMesh != nullptr)
    //{
    //    m_pMeshComponent = new MeshComponent(pMesh, pMaterial);
    //    m_pMeshComponent->SetGameObject(this);
    //    pScene->GetComponentManager()->AddComponent(m_pMeshComponent);
    //}
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
        m_Position = m_pPhysicsBody->GetPosition();
        m_Rotation = m_pPhysicsBody->GetRotation();
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
    CreateBody(pWorld, isDynamic, m_Scale, density);
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density)
{
    m_pPhysicsBody = pWorld->CreateBody(this, isDynamic, size, density, m_Position, m_Rotation);

	if (!m_enabled)
	{
		m_pPhysicsBody->SetState(false);
	}
}

void GameObject::CreateBody(PhysicsWorld* pWorld, bool isDynamic, float radius, float density)
{
	m_pPhysicsBody = pWorld->CreateBody(this, isDynamic, radius, density, m_Position, m_Rotation);

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

const matrix& GameObject::GetWorldTransform()
{
    m_WorldTransform.CreateSRT(m_Scale, m_Rotation, m_Position);
    return m_WorldTransform;
}

void GameObject::SetPosition(vec3 pos)
{
	m_Position = pos; 
	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->SetPosition(pos);
	}
}

void GameObject::SetRotation(vec3 rot)
{
	m_Rotation = rot; 
	if (m_pPhysicsBody)
	{
		m_pPhysicsBody->SetTransform(m_Position, rot);
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
	vec3 pos = m_Position;
	vec3 rot = m_Rotation;
	vec3 scale = m_Scale;
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

		if (pos != m_Position)
		{
			m_pPhysicsBody->SetPosition(pos);
		}
		if (rot != m_Rotation)
		{
			m_pPhysicsBody->SetTransform(pos, rot);
		}
	}
	else
	{
		m_Position = pos;
		m_Rotation = rot;
		m_Scale = scale;
	}

}

} // namespace fw

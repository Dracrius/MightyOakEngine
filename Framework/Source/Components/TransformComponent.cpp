#include "CoreHeaders.h"

#include "TransformComponent.h"
#include "Objects/Mesh.h"

namespace fw {

TransformComponent::TransformComponent(vec3 pos, vec3 rot, vec3 scale) :m_position(pos), m_rotation(rot), m_scale(scale)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::UpdateWorldTransform()
{
	m_worldTransform.CreateSRT(m_scale, m_rotation, m_position);
}
} // namespace fw

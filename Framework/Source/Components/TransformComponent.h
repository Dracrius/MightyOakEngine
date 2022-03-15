#pragma once

#include "Component.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace fw {

class TransformComponent : public Component
{
protected:
	matrix m_worldTransform;
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;

public:
	TransformComponent(vec3 pos, vec3 rot, vec3 scale);
    virtual ~TransformComponent();

    static const char* GetStaticType() { return "TransformComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }

	void UpdateWorldTransform();
	const matrix& GetWorldTransform() const { return m_worldTransform; };

	vec3 GetPosition() { return m_position; }
	vec3 GetRotation() { return m_rotation; }
	vec3 GetScale() { return m_scale; }

	void SetPosition(vec3 pos) { m_position = pos; }
	void SetRotation(vec3 rot) { m_rotation = rot; }
	void SetScale(vec3 scale) { m_scale = scale; }
};

} // namespace fw

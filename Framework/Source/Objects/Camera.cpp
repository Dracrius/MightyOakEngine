#include "CoreHeaders.h"

#include "Camera.h"
#include "Objects/Scene.h"

namespace fw {

Camera::Camera(Scene* pScene, vec3 pos) : GameObject( pScene,  pos, vec3() )
{
}

Camera::Camera(Scene* pScene, vec3 pos, vec3 lookAtPos) : GameObject(pScene, pos, vec3()), m_lookAtPos(lookAtPos), m_lockView(true)
{
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	if (m_shaking)
	{
		m_shakeTimer -= deltaTime;

		if (m_shakeTimer <= 0)
		{
			m_shakeTimer = 0.5f;
			m_shakeOffset = 0.f;
			m_shaking = false;
		}
		else if (m_shakeTimer > 0.375f || m_shakeTimer < 0.125f)
		{
			m_shakeOffset -= deltaTime;
		}
		else if(m_shakeTimer < 0.375f && m_shakeTimer > 0.125f)
		{
			m_shakeOffset += deltaTime;
		}
	}
	if (m_perspectiveMode) //Check that Perspective Mode is enabled
	{
		m_ProjecMatrix.CreatePerspectiveVFoV(45.f, m_aspectRatio, 0.01f, 100.f);
	}
	else //If not use Ortho Projection
	{
		m_ProjecMatrix.CreateOrtho(-5 ,5, -5, 5, -5, 5);
	}

    if (m_pCameraOperator) //Check if the Camera is Attached to an Object ie. has a Camera Operatior
    {
        m_Position = m_pCameraOperator->GetPosition() + vec3(m_offset.x, m_offset.y + m_shakeOffset, m_offset.z);
    }

    if (m_lockView) //Check if the view has been locked to a position
    {
        m_ViewMatrix.CreateLookAtView(m_Position, vec3(0.f, 1.f, 0.f), m_lookAtPos + vec3(0.f, m_shakeOffset, 0.f));
    }
    else //Otherwise look at the Camera's Position
    {
        m_ViewMatrix.CreateLookAtView(m_Position, vec3(0.f, 1.f, 0.f), vec3(m_Position.x, m_Position.y, 0.f) + vec3(0.f, m_shakeOffset, 0.f));
    }
}

matrix Camera::GetViewMatrix()
{
    return m_ViewMatrix;
}

matrix Camera::GetProjecMatrix()
{
    return m_ProjecMatrix;
}

} // namespace fw

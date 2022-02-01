#include "CoreHeaders.h"

#include "Camera.h"
#include "Objects/Scene.h"

namespace fw {

Camera::Camera(Scene* pScene, vec3 pos)
    : GameObject( pScene, nullptr, nullptr, pos, vec3() )
{
    GetViewMatrix();

    //m_ProjecMatrix.CreateOrtho(-5 ,5, -5, 5, -5, 5);
    m_ProjecMatrix.CreatePerspectiveVFoV(45.f, 1.f, 0.01f, 100.f);
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
    m_Position = m_pObjectWeAreFollowing->GetPosition();
}

matrix Camera::GetViewMatrix()
{
    m_ViewMatrix.CreateLookAtView(m_Position, vec3(0.f, 1.f, 0.f), vec3(m_Position.x, m_Position.y, 0.f));

    return m_ViewMatrix;
}

matrix Camera::GetProjecMatrix()
{
    return m_ProjecMatrix;
}

} // namespace fw

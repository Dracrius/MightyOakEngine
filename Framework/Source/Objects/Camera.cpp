#include "CoreHeaders.h"

#include "Camera.h"

namespace fw {

Camera::Camera(GameCore* pGameCore, vec2 pos, vec2 projScale)
    : GameObject( pGameCore, nullptr, nullptr, nullptr, pos )
    , m_ProjectionScale( projScale )
{
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
    m_Position = m_pObjectWeAreFollowing->GetPosition();
}

} // namespace fw

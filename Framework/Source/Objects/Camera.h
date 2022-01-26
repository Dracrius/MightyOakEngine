#pragma once

#include "GameObject.h"

namespace fw {

class Camera : public GameObject
{
public:
    Camera(GameCore* pGameCore, vec3 pos, vec2 projScale);
    virtual ~Camera();

    virtual void Update(float deltaTime) override;

    // Getters.
    vec2 GetProjectionScale() { return m_ProjectionScale; }

    // Setters.
    void SetObjectWeAreFollowing(GameObject* pObj) { m_pObjectWeAreFollowing = pObj; }

protected:
    vec2 m_ProjectionScale;
    GameObject* m_pObjectWeAreFollowing = nullptr;
};

} // namespace fw

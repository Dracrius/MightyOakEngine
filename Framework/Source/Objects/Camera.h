#pragma once

#include "GameObject.h"
#include "Math/Matrix.h"

namespace fw {

class Camera : public GameObject
{
public:
    Camera(Scene* pScene, vec3 pos);
    virtual ~Camera();

    virtual void Update(float deltaTime) override;

    // Getters.
    matrix GetViewMatrix();
    matrix GetProjecMatrix();

    // Setters.
    void SetObjectWeAreFollowing(GameObject* pObj) { m_pObjectWeAreFollowing = pObj; }

protected:
    matrix m_ViewMatrix;
    matrix m_ProjecMatrix;

    GameObject* m_pObjectWeAreFollowing = nullptr;
};

} // namespace fw

#pragma once

#include "GameObject.h"
#include "Math/Matrix.h"

namespace fw {

class Camera : public GameObject
{
public:
    Camera(Scene* pScene, vec3 pos);
    Camera(Scene* pScene, vec3 pos, vec3 lookAtPos);
    virtual ~Camera();

    virtual void Update(float deltaTime) override;

    // Getters.
    matrix GetViewMatrix();
    matrix GetProjecMatrix();

    // Setters.
    void AttachTo(GameObject* pObj) { m_pCameraOperator = pObj; }
    void LockViewToPos(vec3 pos) { m_lookAtPos = pos; m_lockView = true; }
    void LockView() { m_lockView = true; }
    void UnlockView() { m_lockView = false; }

    void SetFPS() { m_offset = 0.f; }
    void SetThirdPerson(vec3 offset) { m_offset = offset; }


protected:
    matrix m_ViewMatrix;
    matrix m_ProjecMatrix;

    GameObject* m_pCameraOperator = nullptr;

    vec3 m_lookAtPos;
    vec3 m_offset;

    bool m_lockView = false;
};

} // namespace fw

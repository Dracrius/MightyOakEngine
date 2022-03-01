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
	float GetAspectRatio() { return m_aspectRatio; }

    // Setters.
    void SetFPS() { m_offset = 0.f; }
    void SetThirdPerson(vec3 offset) { m_offset = offset; }
	void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

	void TogglePerspective() { m_perspectiveMode = !m_perspectiveMode; }
	void SetPerspective(bool perspectiveMode) { m_perspectiveMode = perspectiveMode; }

    void AttachTo(GameObject* pObj) { m_pCameraOperator = pObj; }
    void ClearAttachment() { m_pCameraOperator = nullptr; }
    void LockViewToPos(vec3 pos) { m_lookAtPos = pos; m_lockView = true; }
    void LockView() { m_lockView = true; }
    void UnlockView() { m_lockView = false; }

	void ShakeCamera() { m_shaking = true; }

protected:
    matrix m_ViewMatrix;
    matrix m_ProjecMatrix;

    GameObject* m_pCameraOperator = nullptr;

    vec3 m_lookAtPos;
    vec3 m_offset;

	float m_aspectRatio = 1.f;
	float m_shakeTimer = 0.5f;
	float m_shakeOffset = 0.f;

	bool m_shaking = false;
    bool m_lockView = false;
	bool m_perspectiveMode = true;
};

} // namespace fw

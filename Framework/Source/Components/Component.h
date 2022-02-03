#pragma once

namespace fw {

class GameObject;

class Component
{
public:
    Component();
    virtual ~Component();

    virtual const char* GetType() = 0;

    GameObject* GetGameObject() { return m_pGameObject; }
    virtual void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }

protected:
    GameObject* m_pGameObject = nullptr;
};

} // namespace fw

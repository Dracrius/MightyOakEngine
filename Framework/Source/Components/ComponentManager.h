#pragma once

namespace fw {

class Camera;
class Component;

class ComponentManager
{
public:
    ComponentManager();
    virtual ~ComponentManager() {}

    void Update(float deltaTime);
    void Draw(Camera* pCamera);

    void AddComponent(Component* pComponent);
    void RemoveComponent(Component* pComponent);

protected:
    std::map<const char*, std::vector<Component*>> m_Components;
};

} // namespace fw

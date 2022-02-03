#pragma once

#include "..\Libraries\box2d\include\box2d\box2d.h"
#include "Math/Vector.h"

namespace fw {

class Camera;
class MeshComponent;
class GameCore;
class Mesh;
class Material;
class PhysicsWorld;
class PhysicsBody;
class Scene;

class GameObject
{
public:
    GameObject(Scene* pScene, Mesh* pMesh, Material* pMaterial, vec3 pos, vec3 rot);
    virtual ~GameObject();

    virtual void Update(float deltaTime);
    virtual void Draw(Camera* pCamera);

    void CreateBody(PhysicsWorld* pWorld, bool isDynamic, float density);
    void CreateBody(PhysicsWorld* pWorld, bool isDynamic, vec3 size, float density);

    // Getters.
    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetScale() { return m_Scale; }

    // Setters.
    //void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

    void SetPosition(vec3 pos) { m_Position = pos; }
    void SetRotation(vec3 rot) { m_Rotation = rot; }
    void SetScale(vec3 scale) { m_Scale = scale; }

protected:
    Scene* m_pScene = nullptr;

    //Physics Comp
    PhysicsBody* m_pPhysicsBody = nullptr;

    MeshComponent* m_pMeshComponent = nullptr;

    //Transform Comp
    vec3 m_Position = vec3( 0, 0, 0 );
    vec3 m_Rotation = vec3(0, 0, 0);
    vec3 m_Scale = vec3(1, 1, 1);
};

} // namespace fw

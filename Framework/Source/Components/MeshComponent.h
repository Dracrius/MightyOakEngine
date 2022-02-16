#pragma once

#include "Component.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace fw {

class Camera;
class Mesh;
class Material;

class GameObject;

class MeshComponent : public Component
{
public:
    MeshComponent(Mesh* pMesh, Material* pMaterial);
    virtual ~MeshComponent();

    void Draw(Camera* pCamera, const matrix& worldMat);

    static const char* GetStaticType() { return "MeshComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }

    void SetUVScale(vec2 uvScale) { m_UVScale = uvScale; }
    void SetUVOffset(vec2 uvOffset) { m_UVOffset = uvOffset; }

	void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial;}

    vec2 GetUVScale() { return m_UVScale; }
    vec2 GetUVOffset() { return m_UVOffset; }

protected:
    Mesh* m_pMesh = nullptr;

    Material* m_pMaterial = nullptr;
    vec2 m_UVScale = vec2(1, 1);
    vec2 m_UVOffset = vec2(0, 0);
};

} // namespace fw

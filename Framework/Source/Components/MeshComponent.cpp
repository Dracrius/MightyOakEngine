#include "CoreHeaders.h"

#include "MeshComponent.h"
#include "Objects/Mesh.h"

namespace fw {

MeshComponent::MeshComponent(Mesh* pMesh, Material* pMaterial) : Component(), m_pMesh(pMesh), m_pMaterial(pMaterial)
{
}

MeshComponent::~MeshComponent()
{
}

void MeshComponent::Draw(Camera* pCamera, const matrix& worldMat, const matrix& normalMat)
{
		m_pMesh->Draw(m_pGameObject, pCamera, m_pMaterial, worldMat, normalMat, m_UVScale, m_UVOffset, 0.0f);
}

} // namespace fw

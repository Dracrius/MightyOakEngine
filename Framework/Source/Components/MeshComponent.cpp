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

void MeshComponent::Draw(Camera* pCamera, matrix& worldMat)
{
	m_pMesh->Draw(pCamera, m_pMaterial, worldMat, m_UVScale, m_UVOffset, 0.0f);
}

} // namespace fw

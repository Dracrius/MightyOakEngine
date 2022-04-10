#include "CoreHeaders.h"
#include "DebugDrawBox2D.h"
#include "Objects/Camera.h"
#include "Objects/Material.h"
#include "Objects/Mesh.h"
#include "Math/Matrix.h"

namespace fw {

unsigned char fColortoUChar(float color)
{
	return static_cast<unsigned char>(color * 255);
}

DebugDrawBox2D::~DebugDrawBox2D()
{
	delete m_debugMesh;
}

void DebugDrawBox2D::Draw(Camera* pCamera, Material* pMaterial)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	matrix worldMat;
	worldMat.SetIdentity();

	if (m_debugMesh)
	{
		m_debugMesh->Rebuild(GL_LINES, m_verts);
	}
	else
	{
		if (m_verts.size() > 0)
		{
			m_debugMesh = new Mesh(GL_LINES, m_verts);
		}
	}

	if (m_debugMesh)
	{
		m_debugMesh->Draw(nullptr, pCamera, pMaterial, worldMat, matrix(), vec2(), vec2(), 0.f);

		m_verts.clear();
	}
}

void DebugDrawBox2D::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

}
void DebugDrawBox2D::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i < vertexCount; i++)
	{
		m_verts.push_back({ vec3(vertices[i].x, vertices[i].y, 0.f),  fColortoUChar(color.r), fColortoUChar(color.g), fColortoUChar(color.b), fColortoUChar(0.75f),  vec2() });
		m_verts.push_back({ vec3(vertices[(i + 1) % vertexCount].x, vertices[(i+1) % vertexCount].y, 0.f),  fColortoUChar(color.r), fColortoUChar(color.g), fColortoUChar(color.b), fColortoUChar(0.75f),  vec2() });
	}
}
void DebugDrawBox2D::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{

}
void DebugDrawBox2D::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{

}
void DebugDrawBox2D::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{

}
void DebugDrawBox2D::DrawTransform(const b2Transform& xf)
{

}
void DebugDrawBox2D::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{

}
} // namespace fw

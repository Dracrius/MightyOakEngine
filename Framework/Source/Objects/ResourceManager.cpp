#include "CoreHeaders.h"

#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "SpriteSheet.h"

namespace fw {

ResourceManager::ResourceManager() 
{
	m_Shaders["Default"] = new ShaderProgram("../Framework/Data/Shaders/Default.vert", "../Framework/Data/Shaders/Default.frag"); 
	m_Materials["Default"] = new Material(m_Shaders["Default"], Color4f::Grey());
}

ResourceManager::~ResourceManager()
{
    for (auto& it : m_SpriteSheets)
	{ 
		delete it.second; 
	}
    for (auto& it : m_Materials) 
	{ 
		delete it.second; 
	}
    for (auto& it : m_Shaders) 
	{ 
		delete it.second; 
	}
    for (auto& it : m_Textures)
	{ 
		delete it.second; 
	}
    for (auto& it : m_Meshes) 
	{
		delete it.second;
	}
}

bool ResourceManager::CreateShader(std::string name, const char* vertFilename, const char* fragFilename)
{
	ShaderProgram* shader = new ShaderProgram(vertFilename, fragFilename);

	if (AddShader(name, shader))
	{
		return true;
	}

	delete shader;
	return false;
}

bool ResourceManager::CreateMesh(std::string name)
{
	Mesh* mesh = new Mesh();

	if (AddMesh(name, mesh))
	{
		return true;
	}

	delete mesh;
	return false;
}

bool ResourceManager::CreateMesh(std::string name, GLenum primitiveType, const std::vector<VertexFormat>& verts)
{
	Mesh* mesh = new Mesh(primitiveType, verts);

	if (AddMesh(name, mesh))
	{
		return true;
	}

	delete mesh;
	return false;
}

bool ResourceManager::CreateMesh(std::string name, GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices)
{
	Mesh* mesh = new Mesh(primitiveType, verts, indices);

	if (AddMesh(name, mesh))
	{
		return true;
	}

	delete mesh;
	return false;
}

bool ResourceManager::CreateTexture(std::string name, const char* filename)
{
	Texture* texture = new Texture(filename);

	if (AddTexture(name, texture))
	{
		return true;
	}

	delete texture;
	return false;
}

bool ResourceManager::CreateMaterial(std::string name, Color4f color)
{
	return CreateMaterial(name, m_Shaders["Default"], color);
}

bool ResourceManager::CreateMaterial(std::string name, Texture* pTexture, Color4f color)
{
	return CreateMaterial(name, m_Shaders["Default"], pTexture, color);
}

bool ResourceManager::CreateMaterial(std::string name, ShaderProgram* pShader, Color4f color)
{
	Material* material = new Material(pShader, color);

	if (AddMaterial(name, material))
	{
		return true;
	}

	delete material;
	return false;
}

bool ResourceManager::CreateMaterial(std::string name, ShaderProgram* pShader, Texture* pTexture, Color4f color)
{
	Material* material = new Material(pShader, pTexture, color);

	if (AddMaterial(name, material))
	{
		return true;
	}

	delete material;
	return false;
}

bool ResourceManager::CreateSpriteSheet(std::string name, const char* filename, Texture* pTexture)
{
	SpriteSheet* spriteSheet = new SpriteSheet(filename, pTexture);

	if (AddSpriteSheet(name, spriteSheet))
	{
		return true;
	}

	delete spriteSheet;
	return false;
}

bool ResourceManager::AddShader( std::string name, ShaderProgram* shader)
{
	if (m_Shaders[name])
	{
		return false;
	}

	m_Shaders[name] = shader;
	return true;
}

bool ResourceManager::AddMesh(std::string name, Mesh* mesh)
{
	if (m_Meshes[name])
	{
		return false;
	}
	m_Meshes[name] = mesh; 
	return true;
}

bool ResourceManager::AddTexture(std::string name, Texture* texture)
{ 
	if (m_Textures[name])
	{
		return false;
	}
	m_Textures[name] = texture; 
	return true;
}

bool ResourceManager::AddMaterial(std::string name, Material* material)
{
	if (m_Materials[name])
	{
		return false;
	}
	m_Materials[name] = material; 
	return true;
}

bool ResourceManager::AddSpriteSheet(std::string name, SpriteSheet* spriteSheet)
{ 
	if (m_SpriteSheets[name])
	{
		return false;
	}
	m_SpriteSheets[name] = spriteSheet; 
	return true;
}

bool ResourceManager::RemoveShader(std::string name)
{
    if (m_Shaders[name])
    {
        delete m_Shaders[name];
        return true; 
    }
    return false;
}

bool ResourceManager::RemoveMesh(std::string name)
{
    if (m_Meshes[name])
    {
        delete m_Meshes[name];
        return true;
    }
    return false;
}

bool ResourceManager::RemoveTexture(std::string name)
{
    if (m_Textures[name])
    {
        delete m_Textures[name];
        return true;
    }
    return false;
}

bool ResourceManager::RemoveMaterial(std::string name)
{
    if (m_Materials[name])
    {
        delete m_Materials[name];
        return true;
    }
    return false;
}

bool ResourceManager::RemoveSpriteSheet(std::string name)
{
    if (m_SpriteSheets[name])
    {
        delete m_SpriteSheets[name];
        return true;
    }
    return false;
}

} // namespace fw

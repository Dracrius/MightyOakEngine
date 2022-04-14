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
	m_Shaders["Default"] = new ShaderProgram("Data/FrameworkData/Shaders/Default.vert", "Data/FrameworkData/Shaders/Default.frag"); 
	m_Materials["Default"] = new Material(m_Shaders["Default"], Color4f::Grey());

	RemoveShader("default"); //?
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
	if (m_Shaders.count(name) > 0)
	{
		return false;
	}

	ShaderProgram* shader = new ShaderProgram(vertFilename, fragFilename);
	return AddShader(name, shader);
}

bool ResourceManager::CreateMesh(std::string name)
{
	if (m_Meshes.count(name))
	{
		return false;
	}

	Mesh* mesh = new Mesh();
	return AddMesh(name, mesh);
}

bool ResourceManager::CreateMesh(std::string name, GLenum primitiveType, const std::vector<VertexFormat>& verts)
{
	if (m_Meshes.count(name))
	{
		return false;
	}

	Mesh* mesh = new Mesh(primitiveType, verts);
	return AddMesh(name, mesh);
}

bool ResourceManager::CreateMesh(std::string name, GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices)
{
	if (m_Meshes.count(name))
	{
		return false;
	}

	Mesh* mesh = new Mesh(primitiveType, verts, indices);
	return AddMesh(name, mesh);
}

bool ResourceManager::CreateTexture(std::string name, const char* filename)
{
	if (m_Textures.count(name))
	{
		return false;
	}

	Texture* texture = new Texture(filename);
	return AddTexture(name, texture);
}

bool ResourceManager::CreateTexture(std::string name, std::vector<const char*> filenames)
{
    if (m_Textures.count(name))
    {
        return false;
    }

    Texture* texture = new Texture(filenames);
    return AddTexture(name, texture);
}

bool ResourceManager::CreateMaterial(std::string name, Color4f color)
{
	return CreateMaterial(name, m_Shaders["Default"], color);
}

bool ResourceManager::CreateMaterial(std::string name, Texture* pTexture, Color4f color)
{
	return CreateMaterial(name, m_Shaders["Default"], pTexture, color);
}

bool ResourceManager::CreateMaterial(std::string name, Texture* pTexture, Color4f color, Texture* pCubemap)
{
    return CreateMaterial(name, m_Shaders["Default"], pTexture, color, pCubemap);
}

bool ResourceManager::CreateMaterial(std::string name, ShaderProgram* pShader, Color4f color)
{
	if (m_Materials.count(name))
	{
		return false;
	}

	Material* material = new Material(pShader, color);
	return AddMaterial(name, material);
}

bool ResourceManager::CreateMaterial(std::string name, ShaderProgram* pShader, Texture* pTexture, Color4f color)
{
	if (m_Materials.count(name))
	{
		return false;
	}

	Material* material = new Material(pShader, pTexture, color);
	return AddMaterial(name, material);
}

bool ResourceManager::CreateMaterial(std::string name, ShaderProgram* pShader, Texture* pTexture, Color4f color, Texture* pCubemap)
{
    if (m_Materials.count(name))
    {
        return false;
    }

    Material* material = new Material(pShader, pTexture, color, pCubemap);
    return AddMaterial(name, material);
}

bool ResourceManager::CreateSpriteSheet(std::string name, const char* filename, Texture* pTexture)
{
	if (m_SpriteSheets.count(name))
	{
		return false;
	}

	SpriteSheet* spriteSheet = new SpriteSheet(filename, pTexture);
	return AddSpriteSheet(name, spriteSheet);
}

bool ResourceManager::AddShader( std::string name, ShaderProgram* shader)
{
	if (m_Shaders.count(name))
	{
		return false;
	}

	m_Shaders[name] = shader;
	return true;
}

bool ResourceManager::AddMesh(std::string name, Mesh* mesh)
{
	if (m_Meshes.count(name))
	{
		return false;
	}
	m_Meshes[name] = mesh; 
	return true;
}

bool ResourceManager::AddTexture(std::string name, Texture* texture)
{ 
	if (m_Textures.count(name))
	{
		return false;
	}
	m_Textures[name] = texture; 
	return true;
}

bool ResourceManager::AddMaterial(std::string name, Material* material)
{
	if (m_Materials.count(name))
	{
		return false;
	}
	m_Materials[name] = material; 
	return true;
}

bool ResourceManager::AddSpriteSheet(std::string name, SpriteSheet* spriteSheet)
{ 
	if (m_SpriteSheets.count(name))
	{
		return false;
	}
	m_SpriteSheets[name] = spriteSheet; 
	return true;
}

bool ResourceManager::RemoveShader(std::string name)
{
    if (m_Shaders.count(name))
    {
        delete m_Shaders[name];
		m_Shaders.erase(name);
        return true; 
    }
    return false;
}

bool ResourceManager::RemoveMesh(std::string name)
{
    if (m_Meshes.count(name))
    {
        delete m_Meshes[name];
		m_Meshes.erase(name);
        return true;
    }
    return false;
}

bool ResourceManager::RemoveTexture(std::string name)
{
    if (m_Textures.count(name))
    {
        delete m_Textures[name];
		m_Textures.erase(name);
        return true;
    }
    return false;
}

bool ResourceManager::RemoveMaterial(std::string name)
{
    if (m_Materials.count(name))
    {
        delete m_Materials[name];
		m_Materials.erase(name);
        return true;
    }
    return false;
}

bool ResourceManager::RemoveSpriteSheet(std::string name)
{
    if (m_SpriteSheets.count(name))
    {
        delete m_SpriteSheets[name];
		m_SpriteSheets.erase(name);
        return true;
    }
    return false;
}

} // namespace fw

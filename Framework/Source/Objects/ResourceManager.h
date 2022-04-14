#pragma once
#include <map>
#include <string>

namespace fw {

class Color4f;
class ShaderProgram;
class Mesh;
class Texture;
class Material;
class SpriteSheet;
struct VertexFormat;

class ResourceManager
{
protected:
    std::map<std::string, ShaderProgram*> m_Shaders;
    std::map<std::string, Mesh*> m_Meshes;
    std::map<std::string, Texture*> m_Textures;
    std::map<std::string, Material*> m_Materials;
    std::map<std::string, SpriteSheet*> m_SpriteSheets;
public:
	ResourceManager();
	virtual ~ResourceManager();

	bool CreateShader(std::string name, const char* vertFilename, const char* fragFilename);
	
	bool CreateMesh(std::string name);
	bool CreateMesh(std::string name, GLenum primitiveType, const std::vector<VertexFormat>& verts);
	bool CreateMesh(std::string name, GLenum primitiveType, const std::vector<VertexFormat>& verts, const std::vector<unsigned int>& indices);
	
	bool CreateTexture(std::string name, const char* filename);
    bool CreateTexture(std::string name, std::vector<const char*> filenames);
	
	bool CreateMaterial(std::string name, Color4f color);
	bool CreateMaterial(std::string name, Texture* pTexture, Color4f color);
    bool CreateMaterial(std::string name, Texture* pTexture, Color4f color, Texture* pCubemap);
	bool CreateMaterial(std::string name, ShaderProgram* pShader, Color4f color);
	bool CreateMaterial(std::string name, ShaderProgram* pShader, Texture* pTexture, Color4f color);
    bool CreateMaterial(std::string name, ShaderProgram* pShader, Texture* pTexture, Color4f color, Texture* pCubemap);
	
	bool CreateSpriteSheet(std::string name, const char* filename, Texture* pTexture);
	
	bool AddShader(std::string name, ShaderProgram* shader);
	bool AddMesh(std::string name, Mesh* mesh);
	bool AddTexture(std::string name, Texture* texture);
	bool AddMaterial(std::string name, Material* material);
	bool AddSpriteSheet(std::string name, SpriteSheet* spriteSheet);

    ShaderProgram* GetShader(std::string name) { return m_Shaders[name]; }
    Mesh* GetMesh(std::string name) { return m_Meshes[name]; }
    Texture* GetTexture(std::string name) { return m_Textures[name]; }
    Material* GetMaterial(std::string name) { return m_Materials[name]; }
    SpriteSheet* GetSpriteSheet(std::string name) { return m_SpriteSheets[name]; }

	bool RemoveShader(std::string name);
	bool RemoveMesh(std::string name);
	bool RemoveTexture(std::string name);
	bool RemoveMaterial(std::string name);
	bool RemoveSpriteSheet(std::string name);
};

} // namespace fw

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "glm/glm.hpp"
class Scene;
class GameObject;
class Material;
class DynamicObject;
class Collider;
class Mesh;
class Script;

class SceneLoader 
{
public:
	std::shared_ptr<Scene> LoadScene(const char * _path);
private:
	std::shared_ptr<Scene> LoadScene(std::string& _currentLine, const char* _path);
	void SplitStringWhitespace(std::string& _input, std::vector<std::string>& _output);
	std::shared_ptr<GameObject> LoadGameObject(std::string& _currentLine, std::ifstream& _file);
	std::shared_ptr<DynamicObject> LoadDynamicObject(std::string& _currentLine, std::ifstream& _file);
	glm::vec3 LoadVec3(std::vector<std::string> _tokens);
	std::shared_ptr<Material> LoadMaterial(std::string& _currentLine, std::ifstream& _file);
	std::shared_ptr<Collider> LoadCollider(std::string& _currentLine, std::ifstream& _file);
	glm::vec3 vec3ToRadians(glm::vec3 _input);
	std::shared_ptr<Script> LoadScript(int _index);

	std::vector<std::shared_ptr<Material>> m_materialLibrary;
	std::vector<std::shared_ptr<Mesh>> m_meshLibrary;
};
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "glm/glm.hpp"
class Scene;
class GameObject;
class Material;
class DynamicObject;
struct Collider;
class Mesh;
class Script;

class SceneLoader 
{
public:
	//Load simulation scene from file
	std::shared_ptr<Scene> LoadScene(const char * _path);
private:
	//Load simulation scene from file
	std::shared_ptr<Scene> LoadScene(std::string& _currentLine, const char* _path);
	//Split a string by its whitespace
	void SplitStringWhitespace(std::string& _input, std::vector<std::string>& _output);
	//Load a gameobject from the file
	std::shared_ptr<GameObject> LoadGameObject(std::string& _currentLine, std::ifstream& _file);
	//Load a dynamic object from the file
	std::shared_ptr<DynamicObject> LoadDynamicObject(std::string& _currentLine, std::ifstream& _file);
	//Load a vec 3 from line tokens
	glm::vec3 LoadVec3(std::vector<std::string> _tokens);
	//Load a material from the file
	std::shared_ptr<Material> LoadMaterial(std::string& _currentLine, std::ifstream& _file);
	//Load a collider from the file
	std::shared_ptr<Collider> LoadCollider(std::string& _currentLine, std::ifstream& _file);
	//Convert a vector of degrees to a vector of radians
	glm::vec3 vec3ToRadians(glm::vec3 _input);
	//Load a script from the file
	std::shared_ptr<Script> LoadScript(int _index);

	//Cache of reused elements
	std::vector<std::shared_ptr<Material>> m_materialLibrary;
	std::vector<std::shared_ptr<Mesh>> m_meshLibrary;
};
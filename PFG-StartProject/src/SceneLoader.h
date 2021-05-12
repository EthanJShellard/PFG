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
	std::shared_ptr<Scene> LoadScene(const char * path);
private:
	std::shared_ptr<Scene> LoadScene(std::string& currentLine, const char* path);
	void SplitStringWhitespace(std::string& input, std::vector<std::string>& output);
	std::shared_ptr<GameObject> LoadGameObject(std::string& currentLine, std::ifstream& file);
	std::shared_ptr<DynamicObject> LoadDynamicObject(std::string& currentLine, std::ifstream& file);
	glm::vec3 LoadVec3(std::vector<std::string> tokens);
	std::shared_ptr<Material> LoadMaterial(std::string& currentLine, std::ifstream& file);
	std::shared_ptr<Collider> LoadCollider(std::string& currentLine, std::ifstream& file);
	glm::vec3 vec3ToRadians(glm::vec3 input);
	std::shared_ptr<Script> LoadScript(int index);

	std::vector<std::shared_ptr<Material>> materialLibrary;
	std::vector<std::shared_ptr<Mesh>> meshLibrary;
};
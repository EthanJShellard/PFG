#include "SceneLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "DynamicObject.h"
#include "Collider.h"
#include "Mesh.h"
#include "FlyingCameraController.h"
#include "BallThrowerController.h"
#include "EnemyBallThrower.h"
#include "OrbitController.h"
#include "SimulationStarter.h"
#include <string>
#include <fstream>

std::shared_ptr<Scene> SceneLoader::LoadScene(const char* path)
{
	std::string currentLine = path;

	try 
	{
		return LoadScene(currentLine, path);
	}
	catch (std::exception& e) 
	{
		std::cout << "MESSAGE [" + currentLine + "]"
			<< std::endl;

		throw std::exception();
	}
}

std::shared_ptr<Scene> SceneLoader::LoadScene(std::string& currentLine, const char* path)
{
	//Attempt to open file
	std::ifstream file(path);
	if (!file.is_open()) 
	{
		throw std::exception();
	}

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	materialLibrary = std::vector<std::shared_ptr<Material> >();
	meshLibrary = std::vector<std::shared_ptr<Mesh> >();

	while (!file.eof()) 
	{
		std::getline(file, currentLine);
		if (currentLine.length() < 1) continue; //Skip empty lines

		std::vector<std::string> tokens;
		SplitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "gameobject") 
		{
			scene->AddObject(LoadGameObject(currentLine, file));
		}
		else if (tokens.at(0) == "dynamicobject") 
		{
			scene->AddObject(LoadDynamicObject(currentLine, file));
		}
		else if (tokens.at(0) == "material") 
		{
			materialLibrary.push_back(LoadMaterial(currentLine, file));
		}
		else if (tokens.at(0) == "mesh") 
		{
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
			if (tokens.size() > 1) //Mesh not empty
			{
				mesh->LoadOBJ(tokens.at(1)); //Load mesh using path
			}
			meshLibrary.push_back(mesh);
		}
		else if (tokens.at(0) == "cameraposition") 
		{
			scene->GetCamera()->SetPos(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "cameraangles") 
		{
			scene->GetCamera()->_cameraAngleX = atof(tokens.at(1).c_str());
			scene->GetCamera()->_cameraAngleY = atof(tokens.at(2).c_str());
		}
		else if (tokens.at(0) == "script") 
		{
			scene->AddScript(LoadScript(atoi(tokens.at(1).c_str())));
		}
	}
	//Clean up file stream
	file.close();

	auto goVec = scene->GetObjects();

	scene->Initialize();

	return scene;
}

std::shared_ptr<GameObject> SceneLoader::LoadGameObject(std::string& currentLine, std::ifstream& file)
{
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
	std::vector<std::string> tokens;

	std::string backupLine = currentLine;

	std::getline(file, currentLine);
	SplitStringWhitespace(currentLine, tokens);

	if (tokens.at(0) != "{") //Empty gameobject requested 
	{
		currentLine = backupLine;
		return gameObject;
	}

	while (!file.eof()) 
	{
		std::getline(file, currentLine);
		SplitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "mesh") 
		{
			gameObject->SetMesh(meshLibrary.at(atoi(tokens.at(1).c_str())));
		}
		else if (tokens.at(0) == "simulated") 
		{
			gameObject->SetSimulated(atoi(tokens.at(1).c_str()) == 1); //0 = not simulated, 1 = simulated
		}
		else if (tokens.at(0) == "position") 
		{
			gameObject->SetPosition(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "scale") 
		{
			gameObject->SetScale(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "velocity") 
		{
			gameObject->SetVelocity(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "rotation")
		{
			gameObject->SetRotation(vec3ToRadians(LoadVec3(tokens)));
		}
		else if (tokens.at(0) == "material") 
		{
			gameObject->SetMaterial(materialLibrary.at(atoi(tokens.at(1).c_str())));
		}
		else if (tokens.at(0) == "id") 
		{
			gameObject->ID = atoi(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "collider")
		{
			gameObject->SetCollider(LoadCollider(currentLine, file));
		}
		else if (tokens.at(0) == "}") 
		{
			return gameObject;
		}
	}
	file.close();
	throw std::exception();
}

std::shared_ptr<DynamicObject> SceneLoader::LoadDynamicObject(std::string& currentLine, std::ifstream& file)
{
	std::shared_ptr<DynamicObject> dynamicObject = std::make_shared<DynamicObject>();
	std::vector<std::string> tokens;

	std::string backupLine = currentLine;

	std::getline(file, currentLine);
	SplitStringWhitespace(currentLine, tokens);

	if (tokens.at(0) != "{") //Empty dynamicObject requested 
	{
		currentLine = backupLine;
		return dynamicObject;
	}

	while (!file.eof())
	{
		std::getline(file, currentLine);
		SplitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "mesh")
		{
			dynamicObject->SetMesh(meshLibrary.at(atoi(tokens.at(1).c_str())));
		}
		else if (tokens.at(0) == "simulated")
		{
			dynamicObject->SetSimulated(atoi(tokens.at(1).c_str()) == 1); //0 = not simulated, 1 = simulated
		}
		else if (tokens.at(0) == "position")
		{
			dynamicObject->SetPosition(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "scale")
		{
			dynamicObject->SetScale(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "velocity")
		{
			dynamicObject->SetVelocity(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "rotation")
		{
			dynamicObject->SetRotation(vec3ToRadians(LoadVec3(tokens)));
		}
		else if (tokens.at(0) == "mass") 
		{
			dynamicObject->SetMass(atof(tokens.at(1).c_str()));
		}
		else if (tokens.at(0) == "angularvelocity") 
		{
			dynamicObject->SetAngularVelocity(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "material")
		{
			dynamicObject->SetMaterial(materialLibrary.at(atoi(tokens.at(1).c_str())));
		}
		else if (tokens.at(0) == "id")
		{
			dynamicObject->ID = atoi(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "collider")
		{
			dynamicObject->SetCollider(LoadCollider(currentLine, file));
		}
		else if (tokens.at(0) == "}")
		{
			return dynamicObject;
		}
	}
	file.close();
	throw std::exception();
}

std::shared_ptr<Material> SceneLoader::LoadMaterial(std::string& currentLine, std::ifstream& file)
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	std::vector<std::string> tokens;

	std::string backupLine = currentLine;
	std::string vertPath;
	std::string fragPath;

	std::getline(file, currentLine);
	SplitStringWhitespace(currentLine, tokens);

	if (tokens.at(0) != "{") //Empty material requested 
	{
		currentLine = backupLine;
		return material;
	}

	while (!file.eof())
	{
		std::getline(file, currentLine);
		SplitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "vert") 
		{
			if (tokens.size() > 1) vertPath = tokens.at(1);
			else throw std::exception();

			if (fragPath.length() > 0) 
			{
				material->LoadShaders(vertPath, fragPath);
			}
		}
		else if (tokens.at(0) == "frag") 
		{
			if (tokens.size() > 1) fragPath = tokens.at(1);
			else throw std::exception();

			if (vertPath.length() > 0)
			{
				material->LoadShaders(vertPath, fragPath);
			}
		}
		else if (tokens.at(0) == "colour") 
		{
			material->SetDiffuseColour(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "lightposition") 
		{
			material->SetLightPosition(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "texture") 
		{
			material->SetTexture(tokens.at(1));
		}
		else if (tokens.at(0) == "}") 
		{
			return material;
		}
	}
	file.close();
	throw std::exception();
}

shared_ptr<Collider> SceneLoader::LoadCollider(std::string& currentLine, std::ifstream& file)
{
	std::shared_ptr<Collider> collider;
	std::vector<std::string> tokens;

	ColliderType type;
	std::vector<std::vector<std::string>> parameters;

	std::string backupLine = currentLine;
	std::string vertPath;
	std::string fragPath;
	float bounciness = 0;
	float friction = 0;

	std::getline(file, currentLine);
	SplitStringWhitespace(currentLine, tokens);

	if (tokens.at(0) != "{") //Empty Collider requested, but is impossible 
	{
		currentLine = backupLine;
		throw std::exception();
	}

	while (!file.eof())
	{
		std::getline(file, currentLine);
		SplitStringWhitespace(currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "type")
		{
			if (tokens.size() > 1)
			{
				if (tokens.at(1) == "sphere") type = ColliderType::SPHERE;
				else if (tokens.at(1) == "plane") type = ColliderType::INFINITE_PLANE;
			}
			else throw std::exception();
		}
		else if (tokens.at(0) == "bounciness") 
		{
			bounciness = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "friction") 
		{
			friction = atof(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "}") 
		{
			if (type == ColliderType::SPHERE)
			{
				if (parameters.size() < 1) throw std::exception();
				if (parameters.at(0).size() > 1 && parameters.at(0).at(0) == "radius") collider = std::make_shared<SphereCollider>(atof(parameters.at(0).at(1).c_str()));
				else 
				{
					currentLine = "FAILED TO PROVIDE PLANE COLLIDER PARAMETERS";
					throw std::exception();
				}
			}
			else if (type == ColliderType::INFINITE_PLANE)
			{
				bool normalGot = false;
				bool pointGot = false;

				glm::vec3 normal;
				glm::vec3 point;

				if (parameters.size() < 2) throw std::exception();
				for (int i = 0; i < 2; i++) 
				{
					if (parameters.at(i).at(0) == "normal")
					{
						normal = LoadVec3(parameters.at(i));
						normalGot = true;
					}
					else if (parameters.at(i).at(0) == "point") 
					{
						point = LoadVec3(parameters.at(i));
						pointGot = true;
					}
				}

				if (!pointGot || !normalGot)
				{
					currentLine = "FAILED TO PROVIDE PLANE COLLIDER PARAMETERS";
					throw std::exception();
				}

				collider = std::make_shared<InfinitePlaneCollider>(normal, point);

			}
			else
			{
				currentLine = "NO COLLIDER TYPE SPECIFIED";
				throw std::exception();
			}

			collider->bounciness = bounciness;
			collider->friction = friction;
			return collider;
		}
		else
		{
			if (tokens.size() > 1) 
			{
				std::vector<std::string> v = tokens;
				parameters.push_back(v);
			}
		}
	}
	file.close();
	throw std::exception();

}

glm::vec3 SceneLoader::vec3ToRadians(glm::vec3 input)
{
	glm::vec3 output;
	output.x = glm::radians(input.x);
	output.y = glm::radians(input.y);
	output.z = glm::radians(input.z);
	return output;
}

std::shared_ptr<Script> SceneLoader::LoadScript(int index)
{
	//Select script
	switch (index) 
	{
	case 0: return std::make_shared<FlyingCameraController>(); break;
	case 1: return std::make_shared<BallThrowerController>(); break;
	case 2: return std::make_shared<OrbitController>(); break;
	case 3: return std::make_shared<SimulationStarter>(); break;
	case 4: return std::make_shared<EnemyBallThrower>(); break;
	default: return nullptr;
	}
}

glm::vec3 SceneLoader::LoadVec3(std::vector<std::string> tokens)
{
	if (tokens.size() >= 4)
	{
		glm::vec3 pos;
		pos.x = atof(tokens.at(1).c_str());
		pos.y = atof(tokens.at(2).c_str());
		pos.z = atof(tokens.at(3).c_str());
		return pos;
	}
	else 
	{
		throw std::exception();
	}
}

void SceneLoader::SplitStringWhitespace(std::string& input, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ' ||
			input.at(i) == '\r' ||
			input.at(i) == '\n' ||
			input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

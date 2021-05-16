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

std::shared_ptr<Scene> SceneLoader::LoadScene(const char* _path)
{
	std::string currentLine = _path;

	try 
	{
		return LoadScene(currentLine, _path);
	}
	catch (std::exception& e) 
	{
		std::cout << "MESSAGE [" + currentLine + "]"
			<< std::endl;

		throw std::exception();
	}
}

std::shared_ptr<Scene> SceneLoader::LoadScene(std::string& _currentLine, const char* _path)
{
	//Attempt to open file
	std::ifstream file(_path);
	if (!file.is_open()) 
	{
		throw std::exception();
	}

	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	m_materialLibrary = std::vector<std::shared_ptr<Material> >();
	m_meshLibrary = std::vector<std::shared_ptr<Mesh> >();

	while (!file.eof()) 
	{
		std::getline(file, _currentLine);
		if (_currentLine.length() < 1) continue; //Skip empty lines

		std::vector<std::string> tokens;
		SplitStringWhitespace(_currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "gameobject") 
		{
			scene->AddObject(LoadGameObject(_currentLine, file));
		}
		else if (tokens.at(0) == "dynamicobject") 
		{
			scene->AddObject(LoadDynamicObject(_currentLine, file));
		}
		else if (tokens.at(0) == "material") 
		{
			m_materialLibrary.push_back(LoadMaterial(_currentLine, file));
		}
		else if (tokens.at(0) == "mesh") 
		{
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
			if (tokens.size() > 1) //Mesh not empty
			{
				mesh->LoadOBJ(tokens.at(1)); //Load mesh using path
			}
			m_meshLibrary.push_back(mesh);
		}
		else if (tokens.at(0) == "cameraposition") 
		{
			scene->GetCamera()->SetPos(LoadVec3(tokens));
		}
		else if (tokens.at(0) == "cameraangles") 
		{
			scene->GetCamera()->m_cameraAngleX = atof(tokens.at(1).c_str());
			scene->GetCamera()->m_cameraAngleY = atof(tokens.at(2).c_str());
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

std::shared_ptr<GameObject> SceneLoader::LoadGameObject(std::string& _currentLine, std::ifstream& _file)
{
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
	std::vector<std::string> tokens;

	std::string backupLine = _currentLine;

	std::getline(_file, _currentLine);
	SplitStringWhitespace(_currentLine, tokens);

	if (tokens.at(0) != "{") //Empty gameobject requested 
	{
		_currentLine = backupLine;
		return gameObject;
	}

	while (!_file.eof()) 
	{
		std::getline(_file, _currentLine);
		SplitStringWhitespace(_currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "mesh") 
		{
			gameObject->SetMesh(m_meshLibrary.at(atoi(tokens.at(1).c_str())));
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
			gameObject->SetMaterial(m_materialLibrary.at(atoi(tokens.at(1).c_str())));
		}
		else if (tokens.at(0) == "id") 
		{
			gameObject->ID = atoi(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "collider")
		{
			gameObject->SetCollider(LoadCollider(_currentLine, _file));
		}
		else if (tokens.at(0) == "}") 
		{
			return gameObject;
		}
	}
	_file.close();
	throw std::exception();
}

std::shared_ptr<DynamicObject> SceneLoader::LoadDynamicObject(std::string& _currentLine, std::ifstream& _file)
{
	std::shared_ptr<DynamicObject> dynamicObject = std::make_shared<DynamicObject>();
	std::vector<std::string> tokens;

	std::string backupLine = _currentLine;

	std::getline(_file, _currentLine);
	SplitStringWhitespace(_currentLine, tokens);

	if (tokens.at(0) != "{") //Empty dynamicObject requested 
	{
		_currentLine = backupLine;
		return dynamicObject;
	}

	while (!_file.eof())
	{
		std::getline(_file, _currentLine);
		SplitStringWhitespace(_currentLine, tokens);
		if (tokens.size() < 1) continue; //Skip if no tokens

		if (tokens.at(0) == "mesh")
		{
			dynamicObject->SetMesh(m_meshLibrary.at(atoi(tokens.at(1).c_str())));
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
			dynamicObject->SetMaterial(m_materialLibrary.at(atoi(tokens.at(1).c_str())));
		}
		else if (tokens.at(0) == "id")
		{
			dynamicObject->ID = atoi(tokens.at(1).c_str());
		}
		else if (tokens.at(0) == "collider")
		{
			dynamicObject->SetCollider(LoadCollider(_currentLine, _file));
		}
		else if (tokens.at(0) == "}")
		{
			return dynamicObject;
		}
	}
	_file.close();
	throw std::exception();
}

std::shared_ptr<Material> SceneLoader::LoadMaterial(std::string& _currentLine, std::ifstream& _file)
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	std::vector<std::string> tokens;

	std::string backupLine = _currentLine;
	std::string vertPath;
	std::string fragPath;

	std::getline(_file, _currentLine);
	SplitStringWhitespace(_currentLine, tokens);

	if (tokens.at(0) != "{") //Empty material requested 
	{
		_currentLine = backupLine;
		return material;
	}

	while (!_file.eof())
	{
		std::getline(_file, _currentLine);
		SplitStringWhitespace(_currentLine, tokens);
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
	_file.close();
	throw std::exception();
}

shared_ptr<Collider> SceneLoader::LoadCollider(std::string& _currentLine, std::ifstream& _file)
{
	std::shared_ptr<Collider> collider;
	std::vector<std::string> tokens;

	ColliderType type;
	std::vector<std::vector<std::string>> parameters;

	std::string backupLine = _currentLine;
	std::string vertPath;
	std::string fragPath;
	float bounciness = 0;
	float friction = 0;

	std::getline(_file, _currentLine);
	SplitStringWhitespace(_currentLine, tokens);

	if (tokens.at(0) != "{") //Empty Collider requested, but is impossible 
	{
		_currentLine = backupLine;
		throw std::exception();
	}

	while (!_file.eof())
	{
		std::getline(_file, _currentLine);
		SplitStringWhitespace(_currentLine, tokens);
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
					_currentLine = "FAILED TO PROVIDE PLANE COLLIDER PARAMETERS";
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
					_currentLine = "FAILED TO PROVIDE PLANE COLLIDER PARAMETERS";
					throw std::exception();
				}

				collider = std::make_shared<InfinitePlaneCollider>(normal, point);

			}
			else
			{
				_currentLine = "NO COLLIDER TYPE SPECIFIED";
				throw std::exception();
			}

			collider->m_bounciness = bounciness;
			collider->m_friction = friction;
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
	_file.close();
	throw std::exception();

}

glm::vec3 SceneLoader::vec3ToRadians(glm::vec3 _input)
{
	glm::vec3 output;
	output.x = glm::radians(_input.x);
	output.y = glm::radians(_input.y);
	output.z = glm::radians(_input.z);
	return output;
}

std::shared_ptr<Script> SceneLoader::LoadScript(int _index)
{
	std::shared_ptr<Script> scrp;

	//Select script
	switch (_index) 
	{
	case 0: scrp = std::make_shared<FlyingCameraController>(); break;
	case 1: scrp = std::make_shared<BallThrowerController>(); break;
	case 2: scrp = std::make_shared<OrbitController>(); break;
	case 3: scrp = std::make_shared<SimulationStarter>(); break;
	case 4: scrp = std::make_shared<EnemyBallThrower>(); break;
	default: return nullptr;
	}

	scrp->m_ID = _index;
	return scrp;
}

glm::vec3 SceneLoader::LoadVec3(std::vector<std::string> _tokens)
{
	if (_tokens.size() >= 4)
	{
		glm::vec3 pos;
		pos.x = atof(_tokens.at(1).c_str());
		pos.y = atof(_tokens.at(2).c_str());
		pos.z = atof(_tokens.at(3).c_str());
		return pos;
	}
	else 
	{
		throw std::exception();
	}
}

void SceneLoader::SplitStringWhitespace(std::string& _input, std::vector<std::string>& _output)
{
	std::string curr;

	_output.clear();

	for (size_t i = 0; i < _input.length(); i++)
	{
		if (_input.at(i) == ' ' ||
			_input.at(i) == '\r' ||
			_input.at(i) == '\n' ||
			_input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				_output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += _input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		_output.push_back(curr);
	}
}

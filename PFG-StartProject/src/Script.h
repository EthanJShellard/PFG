#pragma once

class Scene;
class Input;

class Script 
{
public:
	//Unique identifier for this script
	int m_ID;
	//Will this script update?
	bool m_enabled = true;
	//Parent scene
	Scene* m_scene;

	//Pure virtual functions which must be implemented by script subclasses
	virtual void Update(float _deltaTs, Input* _input) = 0;
	virtual void Initialize() = 0;
};
#pragma once

class Scene;
class Input;

class Script 
{
public:
	int ID;
	bool enabled = true;
	Scene* scene;

	virtual void Update(float deltaTs, Input* input) = 0;
	virtual void Initialize() = 0;
};
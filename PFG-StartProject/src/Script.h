#pragma once

class Scene;
class Input;

class Script 
{
public:
	int m_ID;
	bool m_enabled = true;
	Scene* m_scene;

	virtual void Update(float _deltaTs, Input* _input) = 0;
	virtual void Initialize() = 0;
};
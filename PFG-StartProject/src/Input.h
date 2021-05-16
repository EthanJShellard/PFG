#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

using namespace std;


/*! \brief Brief description.
*  Input class captures and processs keyboard and mouse events
*
*/
class Input
{

public:
	Input();
	~Input();


	void update();

	bool m_quit;

	int m_mouse_x, m_mouse_y;

	bool cmd_mouseleft, cmd_mouseleftUp, cmd_mouseright, cmd_mouserightUp;
	bool cmd_s, cmd_a, cmd_d, cmd_w, cmd_z, cmd_x, cmd_c;
	bool m_once;

	glm::vec2 getMouseDelta();
	
private:

	SDL_Event m_eventQueue;
	glm::vec2 m_mouseDelta;

	static bool getKey(int _keyCode);
	static bool getKeyDown(int _keyCode); // Was the key pressed this frame?
	static bool getKeyUp(int _keyCode);

	static std::vector<int> m_keys;
	static std::vector<int> m_upKeys;
	static std::vector<int> m_downKeys;

	bool m_keyDown;

	void keyboard(unsigned char _key);
	void keyboardUp(unsigned char _key);

	void _keyboard(int _key);
	void _keyboardUp(int _key);

};
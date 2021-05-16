#include "Input.h"

/*! \brief Brief description.
*  Input class captures and processs keyboard and mouse events
*
*/
Input::Input()
{
	m_quit = cmd_s = cmd_a = cmd_w = cmd_d = m_keyDown = cmd_mouseleft = cmd_mouseleftUp = cmd_mouseright = cmd_mouserightUp = m_once = cmd_z = cmd_x = cmd_c = false;
}

std::vector<int> Input::m_keys;
std::vector<int> Input::m_downKeys;
std::vector<int> Input::m_upKeys;

Input::~Input()
{
}

glm::vec2 Input::getMouseDelta()
{
	return m_mouseDelta;
}

void Input::update()
{
	m_mouseDelta.x = 0;
	m_mouseDelta.y = 0;
	cmd_mouserightUp = false;
	cmd_mouseleftUp = false;

	while (SDL_PollEvent(&m_eventQueue) != 0)
	{
		if (m_eventQueue.type == SDL_QUIT)
		{
			m_quit = 1;
		}
		else if (m_eventQueue.type == SDL_MOUSEMOTION)
		{

			m_mouse_x = m_eventQueue.motion.x;
			m_mouse_y = m_eventQueue.motion.y;

			m_mouseDelta.x += m_mouse_x - 400;
			m_mouseDelta.y += m_mouse_y - 300;

		}
		else if (m_eventQueue.type == SDL_MOUSEBUTTONDOWN && m_eventQueue.button.button == SDL_BUTTON_LEFT)
		{
			cmd_mouseleft = true;
		}
		else if (m_eventQueue.type == SDL_MOUSEBUTTONDOWN && m_eventQueue.button.button == SDL_BUTTON_RIGHT)
		{
			cmd_mouseright = true;
		}
		else if (m_eventQueue.type == SDL_MOUSEBUTTONUP && m_eventQueue.button.button == SDL_BUTTON_LEFT)
		{
			cmd_mouseleft = false;
			cmd_mouseleftUp = true;
		}
		else if (m_eventQueue.type == SDL_MOUSEBUTTONUP && m_eventQueue.button.button == SDL_BUTTON_RIGHT)
		{
			cmd_mouseright = false;
			cmd_mouserightUp = true;
		}
		else if (m_eventQueue.type == SDL_KEYDOWN)
		{
			if (m_eventQueue.key.keysym.sym == SDLK_UP || m_eventQueue.key.keysym.sym == SDLK_w)
			{
				cmd_w = true;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_DOWN || m_eventQueue.key.keysym.sym == SDLK_s)
			{
				cmd_s = true;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_RIGHT || m_eventQueue.key.keysym.sym == SDLK_d)
			{
				cmd_d = true;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_LEFT || m_eventQueue.key.keysym.sym == SDLK_a)
			{
				cmd_a = true;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_x)
			{
				cmd_x = true;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_ESCAPE)
			{
				m_quit = true;
			}


		}
		else if (m_eventQueue.type == SDL_KEYUP)
		{
			if (m_eventQueue.key.keysym.sym == SDLK_UP || m_eventQueue.key.keysym.sym == SDLK_w )
			{
				//mouseDelta.y = 0;
				cmd_w = false;
				

			}
			else if (m_eventQueue.key.keysym.sym == SDLK_DOWN || m_eventQueue.key.keysym.sym == SDLK_s)
			{
				cmd_s = false;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_RIGHT || m_eventQueue.key.keysym.sym == SDLK_d)
			{
				//mouseDelta.x = 0;
				cmd_d = false;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_LEFT || m_eventQueue.key.keysym.sym == SDLK_a)
			{
				cmd_a = false;
			}
			else if (m_eventQueue.key.keysym.sym == SDLK_x) 
			{
				cmd_x = false;
			}
		}

		/// move the mouse back to the middle of the screen each frame
		SDL_WarpMouseInWindow(NULL, 400, 300);

	}

}






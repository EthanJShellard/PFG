#include "Input.h"

/*! \brief Brief description.
*  Input class captures and processs keyboard and mouse events
*
*/
Input::Input()
{
	Quit = cmd_s = cmd_a = cmd_w = cmd_d = keyDown = cmd_mouseleft = cmd_mouseleftUp = cmd_mouseright = cmd_mouserightUp = once = cmd_z = cmd_x = cmd_c = false;
}

std::vector<int> Input::keys;
std::vector<int> Input::downKeys;
std::vector<int> Input::upKeys;

Input::~Input()
{
}

glm::vec2 Input::getMouseDelta()
{
	return mouseDelta;
}

void Input::update()
{
	mouseDelta.x = 0;
	mouseDelta.y = 0;


	while (SDL_PollEvent(&eventQueue) != 0)
	{
		if (eventQueue.type == SDL_QUIT)
		{
			Quit = 1;
		}
		else if (eventQueue.type == SDL_MOUSEMOTION)
		{

			mouse_x = eventQueue.motion.x;
			mouse_y = eventQueue.motion.y;

			mouseDelta.x += mouse_x - 400;
			mouseDelta.y += mouse_y - 300;

		}
		else if (eventQueue.type == SDL_MOUSEBUTTONDOWN && eventQueue.button.button == SDL_BUTTON_RIGHT)
		{
			mouse_x = eventQueue.motion.x;
			mouse_y = eventQueue.motion.y;

		}
		else if (eventQueue.type == SDL_MOUSEBUTTONUP && eventQueue.button.button == SDL_BUTTON_RIGHT)
		{

			mouseDelta.x = -1;
			mouseDelta.y = -1;
		}
		else if (eventQueue.type == SDL_KEYDOWN)
		{
			if (eventQueue.key.keysym.sym == SDLK_UP || eventQueue.key.keysym.sym == SDLK_w)
			{
				//mouseDelta.y = 10;
				cmd_w = true;
				std::cout << "Down Mouse Button & w ! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_DOWN || eventQueue.key.keysym.sym == SDLK_s)
			{
				//mouseDelta.y = -10;
				cmd_s = true;
				std::cout << "Down Mouse Button & s ! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_RIGHT || eventQueue.key.keysym.sym == SDLK_d)
			{
				//mouseDelta.x = 10;
				cmd_d = true;
				std::cout << "Right Mouse Button & d ! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_LEFT || eventQueue.key.keysym.sym == SDLK_a)
			{
				//mouseDelta.x = -10;
				cmd_a = true;
				std::cout << "Left Mouse Button & a  ! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_x)
			{
				cmd_x = true;
				std::cout << "Left Mouse Button & x  ! \n";
			}
			else if (eventQueue.key.keysym.sym == SDLK_ESCAPE)
			{
				Quit = true;
			}


		}
		else if (eventQueue.type == SDL_KEYUP)
		{
			if (eventQueue.key.keysym.sym == SDLK_UP || eventQueue.key.keysym.sym == SDLK_w ||
				eventQueue.key.keysym.sym == SDLK_DOWN || eventQueue.key.keysym.sym == SDLK_s)
			{
				//mouseDelta.y = 0;
				cmd_w = false;
				cmd_s = false;

			}
			else if (eventQueue.key.keysym.sym == SDLK_RIGHT || eventQueue.key.keysym.sym == SDLK_d ||
				eventQueue.key.keysym.sym == SDLK_LEFT || eventQueue.key.keysym.sym == SDLK_a)
			{
				//mouseDelta.x = 0;
				cmd_a = false;
				cmd_d = false;
			}
		}

		/// move the mouse back to the middle of the screen each frame
		SDL_WarpMouseInWindow(NULL, 400, 300);

	}

}





